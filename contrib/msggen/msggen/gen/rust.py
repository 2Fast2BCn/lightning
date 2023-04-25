from typing import TextIO
from typing import Tuple
from textwrap import dedent, indent
import logging
import sys
import re

from msggen.model import (ArrayField, CompositeField, EnumField,
                          PrimitiveField, Service, Method)
from msggen.gen.generator import IGenerator

logger = logging.getLogger(__name__)

# The following words need to be changed, otherwise they'd clash with
# built-in keywords.
keywords = ["in", "type"]

# Manual overrides for some of the auto-generated types for paths
# Manual overrides for some of the auto-generated types for paths
overrides = {
    'ListPeers.peers[].channels[].state_changes[].old_state': "ChannelState",
    'ListPeers.peers[].channels[].state_changes[].new_state': "ChannelState",
    'ListPeers.peers[].channels[].state_changes[].cause': "ChannelStateChangeCause",
    'ListPeers.peers[].channels[].htlcs[].state': None,
    'ListPeers.peers[].channels[].opener': "ChannelSide",
    'ListPeers.peers[].channels[].closer': "ChannelSide",
    'ListPeers.peers[].channels[].features[]': "string",
    'ListFunds.channels[].state': 'ChannelState',
    'ListTransactions.transactions[].type[]': None,
    'Invoice.exposeprivatechannels': None,
    'ListClosedChannels.closedchannels[].opener': "ChannelSide",
    'ListClosedChannels.closedchannels[].closer': "ChannelSide",
    'ListClosedChannels.closedchannels[].channel_type.names[]': "ChannelType",
}

# A map of schema type to rust primitive types.
typemap = {
    'boolean': 'bool',
    'hex': 'String',
    'msat': 'Amount',
    'msat_or_all': 'AmountOrAll',
    'msat_or_any': 'AmountOrAny',
    'number': 'f64',
    'pubkey': 'PublicKey',
    'short_channel_id': 'ShortChannelId',
    'signature': 'String',
    'string': 'String',
    'txid': 'String',
    'float': 'f32',
    'utxo': 'Utxo',
    'feerate': 'Feerate',
    'outpoint': 'Outpoint',
    'outputdesc': 'OutputDesc',
    'hash': 'Sha256',
    'secret': 'Secret',
}

header = f"""#![allow(non_camel_case_types)]
//! This file was automatically generated using the following command:
//!
//! ```bash
//! {" ".join(sys.argv)}
//! ```
//!
//! Do not edit this file, it'll be overwritten. Rather edit the schema that
//! this file was generated from

"""


def normalize_varname(field):
    """Make sure that the variable name of this field is valid.
    """
    # Dashes are not valid names
    field.path = field.path.replace("-", "_")
    field.path = re.sub(r'(?<!^)(?=[A-Z])', '_', field.path).lower()
    return field


def gen_field(field):
    if isinstance(field, CompositeField):
        return gen_composite(field)
    elif isinstance(field, EnumField):
        return gen_enum(field)
    elif isinstance(field, ArrayField):
        return gen_array(field)
    elif isinstance(field, PrimitiveField):
        return gen_primitive(field)
    else:
        raise ValueError(f"Unmanaged type {field}")


def gen_enum(e):
    defi, decl = "", ""

    if e.path in overrides and overrides[e.path] is None:
        return "", ""

    if e.description != "":
        decl += f"/// {e.description}\n"

    if e.deprecated:
        decl += "#[deprecated]\n"
    decl += f"#[derive(Copy, Clone, Debug, Deserialize, Serialize)]\npub enum {e.typename} {{\n"
    for v in e.variants:
        if v is None:
            continue
        norm = v.normalized()
        decl += f"    #[serde(rename = \"{v}\")]\n"
        decl += f"    {norm},\n"
    decl += "}\n\n"

    # Implement From<i32> so we can convert from the numerical
    # representation
    decl += dedent(f"""\
    impl TryFrom<i32> for {e.typename} {{
        type Error = anyhow::Error;
        fn try_from(c: i32) -> Result<{e.typename}, anyhow::Error> {{
            match c {{
    """)
    for i, v in enumerate(e.variants):
        norm = v.normalized()
        # decl += f"    #[serde(rename = \"{v}\")]\n"
        decl += f"    {i} => Ok({e.typename}::{norm}),\n"
    decl += dedent(f"""\
                o => Err(anyhow::anyhow!("Unknown variant {{}} for enum {e.typename}", o)),
            }}
        }}
    }}
    """)

    typename = e.typename

    if e.path in overrides:
        decl = ""  # No declaration if we have an override
        typename = overrides[e.path]

    if not e.optional:
        defi = f"    // Path `{e.path}`\n"
        defi += rename_if_necessary(str(e.name), e.name.normalized())
        defi += f"    pub {e.name.normalized()}: {typename},\n"
    else:
        defi = f'    #[serde(skip_serializing_if = "Option::is_none")]\n'
        defi += f"    pub {e.name.normalized()}: Option<{typename}>,\n"

    return defi, decl


def gen_primitive(p):
    defi, decl = "", ""
    org = p.name.name
    typename = typemap.get(p.typename, p.typename)
    normalize_varname(p)

    if p.deprecated:
        defi += "    #[deprecated]\n"
    defi += rename_if_necessary(org, p.name.name)
    if not p.optional:
        defi += f"    pub {p.name}: {typename},\n"
    else:
        defi += f"    #[serde(skip_serializing_if = \"Option::is_none\")]\n    pub {p.name}: Option<{typename}>,\n"

    return defi, decl


def rename_if_necessary(original, name):
    if original != name:
        return f"    #[serde(rename = \"{original}\")]\n"
    else:
        return f""


def gen_array(a):
    name = a.name.normalized().replace("[]", "")
    logger.debug(f"Generating array field {a.name} -> {name} ({a.path})")
    _, decl = gen_field(a.itemtype)

    if a.path in overrides:
        decl = ""  # No declaration if we have an override
        itemtype = overrides[a.path]
    elif isinstance(a.itemtype, PrimitiveField):
        itemtype = a.itemtype.typename
    elif isinstance(a.itemtype, CompositeField):
        itemtype = a.itemtype.typename
    elif isinstance(a.itemtype, EnumField):
        itemtype = a.itemtype.typename

    if itemtype is None:
        return ("", "")  # Override said not to include

    itemtype = typemap.get(itemtype, itemtype)
    alias = a.name.normalized()
    defi = ""
    if a.deprecated:
        defi += "    #[deprecated]\n"
    defi += rename_if_necessary(alias, name)
    if not a.optional:
        defi += f"    pub {name}: {'Vec<'*a.dims}{itemtype}{'>'*a.dims},\n"
    else:
        defi += f"    #[serde(skip_serializing_if = \"crate::is_none_or_empty\")]\n    pub {name}: Option<{'Vec<'*a.dims}{itemtype}{'>'*a.dims}>,\n"

    return (defi, decl)


def gen_composite(c) -> Tuple[str, str]:
    logger.debug(f"Generating composite field {c.name} ({c.path})")
    fields = []
    for f in c.fields:
        fields.append(gen_field(f))

    r = "".join([f[1] for f in fields])

    r += f"""#[derive(Clone, Debug, Deserialize, Serialize)]\npub struct {c.typename} {{\n"""

    r += "".join([f[0] for f in fields])

    r += "}\n\n"

    defi = ""
    if c.deprecated:
        defi += "    #[deprecated]\n"
    if not c.optional:
        defi += f"    pub {c.name}: {c.typename},\n"
    else:
        defi += f"    #[serde(skip_serializing_if = \"Option::is_none\")]\n    pub {c.name}: Option<{c.typename}>,\n"

    return defi, r


class RustGenerator(IGenerator):
    def __init__(self, dest: TextIO):
        self.dest = dest

    def write(self, text: str, numindent: int = 0) -> None:
        raw = dedent(text)
        if numindent > 0:
            raw = indent(text, "\t" * numindent)
        self.dest.write(raw)

    def generate_requests(self, service: Service):
        self.write("""\
        pub mod requests {
            #[allow(unused_imports)]
            use crate::primitives::*;
            #[allow(unused_imports)]
            use serde::{{Deserialize, Serialize}};
            use super::{IntoRequest, Request};

        """)

        for meth in service.methods:
            req = meth.request
            _, decl = gen_composite(req)
            self.write(decl, numindent=1)
            self.generate_request_trait_impl(meth)

        self.write("}\n\n")

    def generate_request_trait_impl(self, method: Method):
        self.write(dedent(f"""\
        impl From<{method.request.typename}> for Request {{
            fn from(r: {method.request.typename}) -> Self {{
                Request::{method.name}(r)
            }}
        }}

        impl IntoRequest for {method.request.typename} {{
            type Response = super::responses::{method.response.typename};
        }}

        """), numindent=1)

    def generate_responses(self, service: Service):
        self.write("""
        pub mod responses {
            #[allow(unused_imports)]
            use crate::primitives::*;
            #[allow(unused_imports)]
            use serde::{{Deserialize, Serialize}};
            use super::{TryFromResponseError, Response};

        """)

        for meth in service.methods:
            res = meth.response
            _, decl = gen_composite(res)
            self.write(decl, numindent=1)
            self.generate_response_trait_impl(meth)

        self.write("}\n\n")

    def generate_response_trait_impl(self, method: Method):
        self.write(dedent(f"""\
        impl TryFrom<Response> for {method.response.typename} {{
            type Error = super::TryFromResponseError;

            fn try_from(response: Response) -> Result<Self, Self::Error> {{
                match response {{
                    Response::{method.name}(response) => Ok(response),
                    _ => Err(TryFromResponseError)
                }}
            }}
        }}

        """), numindent=1)

    def generate_enums(self, service: Service):
        """The Request and Response enums serve as parsing primitives.
        """
        self.write(f"""\
        use serde::{{Deserialize, Serialize}};
        pub use requests::*;
        pub use responses::*;

        #[derive(Clone, Debug, Serialize, Deserialize)]
        #[serde(tag = "method", content = "params")]
        #[serde(rename_all = "lowercase")]
        pub enum Request {{
        """)

        for method in service.methods:
            self.write(f"{method.name}(requests::{method.request.typename}),\n", numindent=1)

        self.write(f"""\
        }}

        #[derive(Clone, Debug, Serialize, Deserialize)]
        #[serde(tag = "method", content = "result")]
        #[serde(rename_all = "lowercase")]
        pub enum Response {{
        """)

        for method in service.methods:
            self.write(f"{method.name}(responses::{method.response.typename}),\n", numindent=1)

        self.write(f"""\
        }}

        """)

    def generate_request_trait(self):
        self.write("""
        pub trait IntoRequest: Into<Request> {
            type Response: TryFrom<Response, Error = TryFromResponseError>;
        }

        #[derive(Debug)]
        pub struct TryFromResponseError;

        """)

    def generate(self, service: Service) -> None:
        self.write(header)

        self.generate_enums(service)

        self.generate_request_trait()

        self.generate_requests(service)
        self.generate_responses(service)

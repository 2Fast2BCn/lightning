/* This file was generated by generate-wire.py */
/* Do not modify this file! Modify the _csv file it was generated from. */
/* Original template can be found at tools/gen/header_template */

#ifndef LIGHTNING_WIRE_BOLT12_WIREGEN_H
#define LIGHTNING_WIRE_BOLT12_WIREGEN_H
#include <ccan/tal/tal.h>
#include <wire/tlvstream.h>
#include <wire/wire.h>
#include <bitcoin/short_channel_id.h>
#include <bitcoin/signature.h>
#include <bitcoin/privkey.h>
#include <common/bigsize.h>
#include <common/amount.h>
#include <common/node_id.h>
#include <bitcoin/block.h>
#include <wire/onion_wire.h>


struct blinded_path {
        struct pubkey blinding;
        struct onionmsg_path **path;
};
struct blinded_payinfo {
        u32 fee_base_msat;
        u32 fee_proportional_millionths;
        u16 cltv_expiry_delta;
        u8 *features;
};
struct fallback_address {
        u8 version;
        u8 *address;
};
struct tlv_offer_recurrence {
        u8 time_unit;
        u32 period;
};
struct tlv_offer_recurrence_paywindow {
        u32 seconds_before;
        u8 proportional_amount;
        u32 seconds_after;
};
struct tlv_offer_recurrence_base {
        u8 start_any_period;
        u64 basetime;
};
struct tlv_offer_send_invoice {
};
struct tlv_invoice_send_invoice {
};
struct tlv_invoice_fallbacks {
        struct fallback_address **fallbacks;
};
struct tlv_offer {
        /* Raw fields including unknown ones. */
        struct tlv_field *fields;

	/* TODO The following explicit fields could just point into the
	 * tlv_field entries above to save on memory. */
	struct bitcoin_blkid *chains;
	utf8 *currency;
	u64 *amount;
	utf8 *description;
	u8 *features;
	u64 *absolute_expiry;
	struct blinded_path **paths;
	utf8 *vendor;
	u64 *quantity_min;
	u64 *quantity_max;
        struct tlv_offer_recurrence *recurrence;
        struct tlv_offer_recurrence_paywindow *recurrence_paywindow;
	u32 *recurrence_limit;
        struct tlv_offer_recurrence_base *recurrence_base;
	struct pubkey32 *node_id;
        struct tlv_offer_send_invoice *send_invoice;
	struct sha256 *refund_for;
	struct bip340sig *signature;
};
struct tlv_invoice_request {
        /* Raw fields including unknown ones. */
        struct tlv_field *fields;

	/* TODO The following explicit fields could just point into the
	 * tlv_field entries above to save on memory. */
	struct bitcoin_blkid *chains;
	struct sha256 *offer_id;
	u64 *amount;
	u8 *features;
	u64 *quantity;
	u32 *recurrence_counter;
	u32 *recurrence_start;
	struct pubkey32 *payer_key;
	utf8 *payer_note;
	u8 *payer_info;
	struct sha256 *replace_invoice;
	struct bip340sig *payer_signature;
};
struct tlv_invoice {
        /* Raw fields including unknown ones. */
        struct tlv_field *fields;

	/* TODO The following explicit fields could just point into the
	 * tlv_field entries above to save on memory. */
	struct bitcoin_blkid *chains;
	struct sha256 *offer_id;
	u64 *amount;
	utf8 *description;
	u8 *features;
	struct blinded_path **paths;
	struct blinded_payinfo **blindedpay;
	struct amount_msat *blinded_capacities;
	utf8 *vendor;
	struct pubkey32 *node_id;
	u64 *quantity;
	struct sha256 *refund_for;
	u32 *recurrence_counter;
        struct tlv_invoice_send_invoice *send_invoice;
	u32 *recurrence_start;
	u64 *recurrence_basetime;
	struct pubkey32 *payer_key;
	utf8 *payer_note;
	u8 *payer_info;
	u64 *timestamp;
	struct sha256 *payment_hash;
	u32 *relative_expiry;
	u32 *cltv;
        struct tlv_invoice_fallbacks *fallbacks;
	struct bip340sig *refund_signature;
	struct sha256 *replace_invoice;
	struct bip340sig *signature;
};
struct tlv_invoice_error {
        /* Raw fields including unknown ones. */
        struct tlv_field *fields;

	/* TODO The following explicit fields could just point into the
	 * tlv_field entries above to save on memory. */
	u64 *erroneous_field;
	u8 *suggested_value;
	utf8 *error;
};

struct tlv_offer *tlv_offer_new(const tal_t *ctx);

/**
 * Deserialize a TLV stream for the offer namespace.
 *
 * This function will parse any TLV stream, as long as the type, length and
 * value fields are formatted correctly. Fields that are not known in the
 * current namespace are stored in the `fields` member. Validity can be
 * checked using offer_is_valid.
 */
bool fromwire_offer(const u8 **cursor, size_t *max,
			  struct tlv_offer * record);

/**
 * Serialize a TLV stream for the offer namespace.
 *
 * This function only considers known fields from the offer namespace,
 * and will ignore any fields that may be stored in the `fields` member. This
 * ensures that the resulting stream is valid according to
 * `offer_is_valid`.
 */
void towire_offer(u8 **pptr, const struct tlv_offer *record);

/**
 * Check that the TLV stream is valid.
 *
 * Enforces the followin validity rules:
 * - Types must be in monotonic non-repeating order
 * - We must understand all even types
 *
 * Returns false if an error was detected, otherwise returns true. If err_index
 * is non-null and we detect an error it is set to the index of the first error
 * detected.
 */
bool offer_is_valid(const struct tlv_offer *record,
			  size_t *err_index);

struct tlv_invoice_request *tlv_invoice_request_new(const tal_t *ctx);

/**
 * Deserialize a TLV stream for the invoice_request namespace.
 *
 * This function will parse any TLV stream, as long as the type, length and
 * value fields are formatted correctly. Fields that are not known in the
 * current namespace are stored in the `fields` member. Validity can be
 * checked using invoice_request_is_valid.
 */
bool fromwire_invoice_request(const u8 **cursor, size_t *max,
			  struct tlv_invoice_request * record);

/**
 * Serialize a TLV stream for the invoice_request namespace.
 *
 * This function only considers known fields from the invoice_request namespace,
 * and will ignore any fields that may be stored in the `fields` member. This
 * ensures that the resulting stream is valid according to
 * `invoice_request_is_valid`.
 */
void towire_invoice_request(u8 **pptr, const struct tlv_invoice_request *record);

/**
 * Check that the TLV stream is valid.
 *
 * Enforces the followin validity rules:
 * - Types must be in monotonic non-repeating order
 * - We must understand all even types
 *
 * Returns false if an error was detected, otherwise returns true. If err_index
 * is non-null and we detect an error it is set to the index of the first error
 * detected.
 */
bool invoice_request_is_valid(const struct tlv_invoice_request *record,
			  size_t *err_index);

#define TLVS_ARRAY_SIZE_invoice_request 12
extern const struct tlv_record_type tlvs_invoice_request[];



/* Define an enum with the constants */
enum invoice_request_types {
	TLV_INVOICE_REQUEST_CHAINS = 2,
	TLV_INVOICE_REQUEST_OFFER_ID = 4,
	TLV_INVOICE_REQUEST_AMOUNT = 8,
	TLV_INVOICE_REQUEST_FEATURES = 12,
	TLV_INVOICE_REQUEST_QUANTITY = 32,
	TLV_INVOICE_REQUEST_RECURRENCE_COUNTER = 36,
	TLV_INVOICE_REQUEST_PAYER_KEY = 38,
	TLV_INVOICE_REQUEST_PAYER_NOTE = 39,
	TLV_INVOICE_REQUEST_PAYER_INFO = 50,
	TLV_INVOICE_REQUEST_REPLACE_INVOICE = 56,
	TLV_INVOICE_REQUEST_RECURRENCE_START = 68,
	TLV_INVOICE_REQUEST_PAYER_SIGNATURE = 240,
};

struct tlv_invoice *tlv_invoice_new(const tal_t *ctx);

/**
 * Deserialize a TLV stream for the invoice namespace.
 *
 * This function will parse any TLV stream, as long as the type, length and
 * value fields are formatted correctly. Fields that are not known in the
 * current namespace are stored in the `fields` member. Validity can be
 * checked using invoice_is_valid.
 */
bool fromwire_invoice(const u8 **cursor, size_t *max,
			  struct tlv_invoice * record);

/**
 * Serialize a TLV stream for the invoice namespace.
 *
 * This function only considers known fields from the invoice namespace,
 * and will ignore any fields that may be stored in the `fields` member. This
 * ensures that the resulting stream is valid according to
 * `invoice_is_valid`.
 */
void towire_invoice(u8 **pptr, const struct tlv_invoice *record);

/**
 * Check that the TLV stream is valid.
 *
 * Enforces the followin validity rules:
 * - Types must be in monotonic non-repeating order
 * - We must understand all even types
 *
 * Returns false if an error was detected, otherwise returns true. If err_index
 * is non-null and we detect an error it is set to the index of the first error
 * detected.
 */
bool invoice_is_valid(const struct tlv_invoice *record,
			  size_t *err_index);

struct tlv_invoice_error *tlv_invoice_error_new(const tal_t *ctx);

/**
 * Deserialize a TLV stream for the invoice_error namespace.
 *
 * This function will parse any TLV stream, as long as the type, length and
 * value fields are formatted correctly. Fields that are not known in the
 * current namespace are stored in the `fields` member. Validity can be
 * checked using invoice_error_is_valid.
 */
bool fromwire_invoice_error(const u8 **cursor, size_t *max,
			  struct tlv_invoice_error * record);

/**
 * Serialize a TLV stream for the invoice_error namespace.
 *
 * This function only considers known fields from the invoice_error namespace,
 * and will ignore any fields that may be stored in the `fields` member. This
 * ensures that the resulting stream is valid according to
 * `invoice_error_is_valid`.
 */
void towire_invoice_error(u8 **pptr, const struct tlv_invoice_error *record);

/**
 * Check that the TLV stream is valid.
 *
 * Enforces the followin validity rules:
 * - Types must be in monotonic non-repeating order
 * - We must understand all even types
 *
 * Returns false if an error was detected, otherwise returns true. If err_index
 * is non-null and we detect an error it is set to the index of the first error
 * detected.
 */
bool invoice_error_is_valid(const struct tlv_invoice_error *record,
			  size_t *err_index);

/* SUBTYPE: BLINDED_PATH */
void towire_blinded_path(u8 **p, const struct blinded_path *blinded_path);
struct blinded_path *fromwire_blinded_path(const tal_t *ctx, const u8 **cursor, size_t *plen);

/* SUBTYPE: BLINDED_PAYINFO */
void towire_blinded_payinfo(u8 **p, const struct blinded_payinfo *blinded_payinfo);
struct blinded_payinfo *fromwire_blinded_payinfo(const tal_t *ctx, const u8 **cursor, size_t *plen);

/* SUBTYPE: FALLBACK_ADDRESS */
void towire_fallback_address(u8 **p, const struct fallback_address *fallback_address);
struct fallback_address *fromwire_fallback_address(const tal_t *ctx, const u8 **cursor, size_t *plen);


#endif /* LIGHTNING_WIRE_BOLT12_WIREGEN_H */
// SHA256STAMP:27ffc38bc2be76e159508470734655f35e59d82927beb8c1f62917e592d76d10

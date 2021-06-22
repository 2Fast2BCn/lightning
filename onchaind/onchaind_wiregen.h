/* This file was generated by generate-wire.py */
/* Do not modify this file! Modify the _csv file it was generated from. */
/* Original template can be found at tools/gen/header_template */

#ifndef LIGHTNING_ONCHAIND_ONCHAIND_WIREGEN_H
#define LIGHTNING_ONCHAIND_ONCHAIND_WIREGEN_H
#include <ccan/tal/tal.h>
#include <wire/tlvstream.h>
#include <wire/wire.h>
#include <bitcoin/tx_parts.h>
#include <common/coin_mvt.h>
#include <common/derive_basepoints.h>
#include <common/htlc_wire.h>
#include <common/wallet.h>
#include <onchaind/onchaind_wire.h>

enum onchaind_wire {
        /*  Begin!  Here's the onchain tx which spends funding tx */
        WIRE_ONCHAIND_INIT = 5001,
        /*  This is all the HTLCs: one per message */
        WIRE_ONCHAIND_HTLC = 5002,
        /*  This says we're ready; give us preimages. */
        WIRE_ONCHAIND_INIT_REPLY = 5101,
        /*  onchaind->master: Send out a tx. */
        /*  If is_rbf is false then master should rebroadcast the tx. */
        /*  If is_rbf is true then onchaind is responsible for rebroadcasting */
        /*   it with a higher fee. */
        WIRE_ONCHAIND_BROADCAST_TX = 5003,
        /*  master->onchaind: Notifier that an output has been spent by input_num of tx. */
        WIRE_ONCHAIND_SPENT = 5004,
        /*  master->onchaind: We will receive more than one of these */
        WIRE_ONCHAIND_DEPTH = 5005,
        /*  onchaind->master: We don't want to watch this tx */
        WIRE_ONCHAIND_UNWATCH_TX = 5006,
        /*  master->onchaind: We know HTLC preimage */
        WIRE_ONCHAIND_KNOWN_PREIMAGE = 5007,
        /*  onchaind->master: We discovered HTLC preimage */
        WIRE_ONCHAIND_EXTRACTED_PREIMAGE = 5008,
        /*  onchaind->master: this HTLC was missing from commit tx. */
        WIRE_ONCHAIND_MISSING_HTLC_OUTPUT = 5009,
        /*  onchaind->master: this HTLC has timed out (after reasonable_depth) */
        WIRE_ONCHAIND_HTLC_TIMEOUT = 5010,
        /*  onchaind->master: this peer can be forgotten */
        WIRE_ONCHAIND_ALL_IRREVOCABLY_RESOLVED = 5011,
        /*  onchaind->master: hey */
        WIRE_ONCHAIND_ADD_UTXO = 5012,
        /*  master -> onchaind: do you have a memleak? */
        WIRE_ONCHAIND_DEV_MEMLEAK = 5033,
        WIRE_ONCHAIND_DEV_MEMLEAK_REPLY = 5133,
        /*  Tell the main daemon what we've been watching */
        /*  that we tracked automatically but only onchaind knows how to classify their */
        /*  transactions. */
        WIRE_ONCHAIND_ANNOTATE_TXOUT = 5035,
        WIRE_ONCHAIND_ANNOTATE_TXIN = 5036,
        WIRE_ONCHAIND_NOTIFY_COIN_MVT = 5037,
};

const char *onchaind_wire_name(int e);

/**
 * Determine whether a given message type is defined as a message.
 *
 * Returns true if the message type is part of the message definitions we have
 * generated parsers for, false if it is a custom message that cannot be
 * handled internally.
 */
bool onchaind_wire_is_defined(u16 type);


/* WIRE: ONCHAIND_INIT */
/*  Begin!  Here's the onchain tx which spends funding tx */
u8 *towire_onchaind_init(const tal_t *ctx, const struct shachain *shachain, const struct chainparams *chainparams, struct amount_sat funding_amount_satoshi, struct amount_msat our_msat, const struct pubkey *old_remote_per_commitment_point, const struct pubkey *remote_per_commitment_point, u32 local_to_self_delay, u32 remote_to_self_delay, u32 delayed_to_us_feerate, u32 htlc_feerate, u32 penalty_feerate, struct amount_sat local_dust_limit_satoshi, const struct bitcoin_txid *our_broadcast_txid, const u8 *local_scriptpubkey, const u8 *remote_scriptpubkey, const struct pubkey *ourwallet_pubkey, enum side opener, const struct basepoints *local_basepoints, const struct basepoints *remote_basepoints, const struct tx_parts *tx_parts, u32 locktime, u32 tx_blockheight, u32 reasonable_depth, const struct bitcoin_signature *htlc_signature, u64 num_htlcs, u32 min_possible_feerate, u32 max_possible_feerate, const struct pubkey *possible_remote_per_commit_point, const struct pubkey *local_funding_pubkey, const struct pubkey *remote_funding_pubkey, u64 local_static_remotekey_start, u64 remote_static_remotekey_start, bool option_anchor_outputs, bool is_replay, u32 min_relay_feerate);
bool fromwire_onchaind_init(const tal_t *ctx, const void *p, struct shachain *shachain, const struct chainparams **chainparams, struct amount_sat *funding_amount_satoshi, struct amount_msat *our_msat, struct pubkey *old_remote_per_commitment_point, struct pubkey *remote_per_commitment_point, u32 *local_to_self_delay, u32 *remote_to_self_delay, u32 *delayed_to_us_feerate, u32 *htlc_feerate, u32 *penalty_feerate, struct amount_sat *local_dust_limit_satoshi, struct bitcoin_txid *our_broadcast_txid, u8 **local_scriptpubkey, u8 **remote_scriptpubkey, struct pubkey *ourwallet_pubkey, enum side *opener, struct basepoints *local_basepoints, struct basepoints *remote_basepoints, struct tx_parts **tx_parts, u32 *locktime, u32 *tx_blockheight, u32 *reasonable_depth, struct bitcoin_signature **htlc_signature, u64 *num_htlcs, u32 *min_possible_feerate, u32 *max_possible_feerate, struct pubkey **possible_remote_per_commit_point, struct pubkey *local_funding_pubkey, struct pubkey *remote_funding_pubkey, u64 *local_static_remotekey_start, u64 *remote_static_remotekey_start, bool *option_anchor_outputs, bool *is_replay, u32 *min_relay_feerate);

/* WIRE: ONCHAIND_HTLC */
/*  This is all the HTLCs: one per message */
u8 *towire_onchaind_htlc(const tal_t *ctx, const struct htlc_stub *htlc, bool tell_if_missing, bool tell_immediately);
bool fromwire_onchaind_htlc(const void *p, struct htlc_stub *htlc, bool *tell_if_missing, bool *tell_immediately);

/* WIRE: ONCHAIND_INIT_REPLY */
/*  This says we're ready; give us preimages. */
u8 *towire_onchaind_init_reply(const tal_t *ctx);
bool fromwire_onchaind_init_reply(const void *p);

/* WIRE: ONCHAIND_BROADCAST_TX */
/*  onchaind->master: Send out a tx. */
/*  If is_rbf is false then master should rebroadcast the tx. */
/*  If is_rbf is true then onchaind is responsible for rebroadcasting */
/*   it with a higher fee. */
u8 *towire_onchaind_broadcast_tx(const tal_t *ctx, const struct bitcoin_tx *tx, enum wallet_tx_type type, bool is_rbf);
bool fromwire_onchaind_broadcast_tx(const tal_t *ctx, const void *p, struct bitcoin_tx **tx, enum wallet_tx_type *type, bool *is_rbf);

/* WIRE: ONCHAIND_SPENT */
/*  master->onchaind: Notifier that an output has been spent by input_num of tx. */
u8 *towire_onchaind_spent(const tal_t *ctx, const struct tx_parts *tx, u32 input_num, u32 blockheight, bool is_replay);
bool fromwire_onchaind_spent(const tal_t *ctx, const void *p, struct tx_parts **tx, u32 *input_num, u32 *blockheight, bool *is_replay);

/* WIRE: ONCHAIND_DEPTH */
/*  master->onchaind: We will receive more than one of these */
u8 *towire_onchaind_depth(const tal_t *ctx, const struct bitcoin_txid *txid, u32 depth, bool is_replay);
bool fromwire_onchaind_depth(const void *p, struct bitcoin_txid *txid, u32 *depth, bool *is_replay);

/* WIRE: ONCHAIND_UNWATCH_TX */
/*  onchaind->master: We don't want to watch this tx */
u8 *towire_onchaind_unwatch_tx(const tal_t *ctx, const struct bitcoin_txid *txid);
bool fromwire_onchaind_unwatch_tx(const void *p, struct bitcoin_txid *txid);

/* WIRE: ONCHAIND_KNOWN_PREIMAGE */
/*  master->onchaind: We know HTLC preimage */
u8 *towire_onchaind_known_preimage(const tal_t *ctx, const struct preimage *preimage, bool is_replay);
bool fromwire_onchaind_known_preimage(const void *p, struct preimage *preimage, bool *is_replay);

/* WIRE: ONCHAIND_EXTRACTED_PREIMAGE */
/*  onchaind->master: We discovered HTLC preimage */
u8 *towire_onchaind_extracted_preimage(const tal_t *ctx, const struct preimage *preimage);
bool fromwire_onchaind_extracted_preimage(const void *p, struct preimage *preimage);

/* WIRE: ONCHAIND_MISSING_HTLC_OUTPUT */
/*  onchaind->master: this HTLC was missing from commit tx. */
u8 *towire_onchaind_missing_htlc_output(const tal_t *ctx, const struct htlc_stub *htlc);
bool fromwire_onchaind_missing_htlc_output(const void *p, struct htlc_stub *htlc);

/* WIRE: ONCHAIND_HTLC_TIMEOUT */
/*  onchaind->master: this HTLC has timed out (after reasonable_depth) */
u8 *towire_onchaind_htlc_timeout(const tal_t *ctx, const struct htlc_stub *htlc);
bool fromwire_onchaind_htlc_timeout(const void *p, struct htlc_stub *htlc);

/* WIRE: ONCHAIND_ALL_IRREVOCABLY_RESOLVED */
/*  onchaind->master: this peer can be forgotten */
u8 *towire_onchaind_all_irrevocably_resolved(const tal_t *ctx);
bool fromwire_onchaind_all_irrevocably_resolved(const void *p);

/* WIRE: ONCHAIND_ADD_UTXO */
/*  onchaind->master: hey */
u8 *towire_onchaind_add_utxo(const tal_t *ctx, const struct bitcoin_txid *prev_out_tx, u32 prev_out_index, const struct pubkey *per_commit_point, struct amount_sat value, u32 blockheight, const u8 *scriptpubkey);
bool fromwire_onchaind_add_utxo(const tal_t *ctx, const void *p, struct bitcoin_txid *prev_out_tx, u32 *prev_out_index, struct pubkey **per_commit_point, struct amount_sat *value, u32 *blockheight, u8 **scriptpubkey);

/* WIRE: ONCHAIND_DEV_MEMLEAK */
/*  master -> onchaind: do you have a memleak? */
u8 *towire_onchaind_dev_memleak(const tal_t *ctx);
bool fromwire_onchaind_dev_memleak(const void *p);

/* WIRE: ONCHAIND_DEV_MEMLEAK_REPLY */
u8 *towire_onchaind_dev_memleak_reply(const tal_t *ctx, bool leak);
bool fromwire_onchaind_dev_memleak_reply(const void *p, bool *leak);

/* WIRE: ONCHAIND_ANNOTATE_TXOUT */
/*  Tell the main daemon what we've been watching */
/*  that we tracked automatically but only onchaind knows how to classify their */
/*  transactions. */
u8 *towire_onchaind_annotate_txout(const tal_t *ctx, const struct bitcoin_txid *txid, u32 outnum, enum wallet_tx_type type);
bool fromwire_onchaind_annotate_txout(const void *p, struct bitcoin_txid *txid, u32 *outnum, enum wallet_tx_type *type);

/* WIRE: ONCHAIND_ANNOTATE_TXIN */
u8 *towire_onchaind_annotate_txin(const tal_t *ctx, const struct bitcoin_txid *txid, u32 innum, enum wallet_tx_type type);
bool fromwire_onchaind_annotate_txin(const void *p, struct bitcoin_txid *txid, u32 *innum, enum wallet_tx_type *type);

/* WIRE: ONCHAIND_NOTIFY_COIN_MVT */
u8 *towire_onchaind_notify_coin_mvt(const tal_t *ctx, const struct chain_coin_mvt *mvt);
bool fromwire_onchaind_notify_coin_mvt(const void *p, struct chain_coin_mvt *mvt);


#endif /* LIGHTNING_ONCHAIND_ONCHAIND_WIREGEN_H */
// SHA256STAMP:40630b923c5c303627b75b8ca663b0d8a060039d2171571f6ecab580695dfdba

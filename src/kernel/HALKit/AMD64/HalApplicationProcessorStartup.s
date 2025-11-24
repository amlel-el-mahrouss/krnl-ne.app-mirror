 /*
 *	========================================================
 *
 *	NeKernel
 * 	Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
 *
 * 	========================================================
 */

.text

.global hal_ap_blob_start         
.global hal_ap_blob_length          

hal_ap_blob_start:
    cli
    hlt
    jmp hal_ap_blob_start

.data

hal_ap_blob_length:
    .long 4

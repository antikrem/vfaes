
PUBLIC initialise_encryption_env
PUBLIC encrypt
PUBLIC initialise_decryption_env
PUBLIC decrypt

EXTERN puts:PROC

.data

    hello1 db "Hello from ASM.",0
	
	nonce  OWORD    0                  ; Keeps 128bit nonce

	key0   OWORD	0                  ; Keeps 128bit key from first pass
	                                   ; Rest is the key schedule

    key1   OWORD	0 
	key2   OWORD	0 
	key3   OWORD	0 
	key4   OWORD	0 
	key5   OWORD	0 
	key6   OWORD	0 
	key7   OWORD	0 
	key8   OWORD	0 
	key9   OWORD	0 
	key10  OWORD	0 

.code

; Generates a new key from last round key and AESKEYGENASSIST
; Generally: for this to generate key_n, xmm0 must contain key_{n-1}
generate_key_ninverse MACRO rcon, dest
    aeskeygenassist xmm1, xmm0, rcon   ; Create key assist and keep in xmm1

	pshufd xmm1, xmm1, 11111111b

	shufps xmm2, xmm0, 00010000b

	pxor  xmm0, xmm2

	shufps xmm2, xmm0, 10001100b

	pxor  xmm0, xmm2

	pxor  xmm0, xmm1
	
	movaps dest, xmm0

ENDM

; Generally: for this to generate key_n, xmm0 must contain key_{n-1}
generate_key_inverse MACRO rcon, dest
    aeskeygenassist xmm1, xmm0, rcon   ; Create key assist and keep in xmm1

	pshufd xmm1, xmm1, 11111111b

	shufps xmm2, xmm0, 00010000b

	pxor  xmm0, xmm2

	shufps xmm2, xmm0, 10001100b

	pxor  xmm0, xmm2

	pxor  xmm0, xmm1
	
	aesimc xmm3, xmm0
	movdqa dest, xmm3

ENDM

; Generates and fills key schedule
; Requires zeroth key to be in xmm0
generate_key_encrypt_schdule PROC
	push rbp
    mov rbp, rsp
	 
	movaps xmm0, key0                  ; Set xmm0 to be starting key
	pxor xmm2, xmm2                    ; Clear xmm2                         

	generate_key_ninverse 1,   key1
	generate_key_ninverse 2,   key2
	generate_key_ninverse 4,   key3
	generate_key_ninverse 8,   key4
	generate_key_ninverse 16,  key5
	generate_key_ninverse 32,  key6
	generate_key_ninverse 64,  key7
	generate_key_ninverse 128, key8
	generate_key_ninverse 27,  key9
	generate_key_ninverse 54,  key10

	leave
    ret
generate_key_encrypt_schdule ENDP


; Sets up environment for encrypting
; Called with function signature: void(int128* nonce, int128* key)
; Sets nonce and inkey in data segment
; Also generates all key rows in data segment
initialise_encryption_env PROC
    push rbp
    mov rbp, rsp
    sub rsp, 32                        ; Shadow Space
    and spl, -16                       ; Align stack at 16
	
	movdqu xmm0, [ rcx ]	           ; Move second parameter to temporary registar (128 bits)
	movdqu key0, xmm0                  ; Move temporary registar to key0 (128 bits)

	movdqu xmm0, [ rdx ]	           ; Move first parameter to temporary registar (128 bits)
	movdqu nonce, xmm0                 ; Move temporary registar to nonce (128 bits)


	call generate_key_encrypt_schdule  ; Call generate_key_encrypt_schdule to populate the schedule
   
	leave                              ; Restore and return
    ret

initialise_encryption_env ENDP


; Procedure for encrypting a 128 bit block using AES in CTR mode
; Called with function signature void(void* )
; RCX <- Starting address of block
; RDX <- Counter for block
encrypt PROC
    push       rbp
    mov        rbp, rsp
    sub        rsp, 32                 ; Shadow Space
    and        spl, -16                ; Align stack at 16

	movdqu     xmm0, [ rcx ]           ; Load block onto register

	pxor       xmm0, key0              ; Begin encryption rounds on xmm0
	aesenc     xmm0, key1
    aesenc     xmm0, key2
    aesenc     xmm0, key3
    aesenc     xmm0, key4
    aesenc     xmm0, key5
    aesenc     xmm0, key6
    aesenc     xmm0, key7
    aesenc     xmm0, key8
    aesenc     xmm0, key9
    aesenclast xmm0, key10

	movdqu [ rcx ], xmm0

    leave                              ; Restore stack (rsp) & frame pointer (rbp)
    ret
encrypt ENDP

; Generates and fills key schedule for decryption
; Requires zeroth key to be in xmm0
generate_key_decrypt_schdule PROC
	push rbp
    mov rbp, rsp
	 
	movaps xmm0, key0                  ; Set xmm0 to be starting key
	pxor xmm2, xmm2                    ; Clear xmm2                         

	generate_key_inverse 1,   key1
	generate_key_inverse 2,   key2
	generate_key_inverse 4,   key3
	generate_key_inverse 8,   key4
	generate_key_inverse 16,  key5
	generate_key_inverse 32,  key6
	generate_key_inverse 64,  key7
	generate_key_inverse 128, key8
	generate_key_inverse 27,  key9
	generate_key_ninverse 54,  key10

	leave
    ret
generate_key_decrypt_schdule ENDP


; Sets up environment for encrypting
; Called with function signature: void(int128* nonce, int128* key)
; Sets nonce and inkey in data segment
; Also generates all key rows in data segment
initialise_decryption_env PROC
    push rbp
    mov rbp, rsp
    sub rsp, 32                        ; Shadow Space
    and spl, -16                       ; Align stack at 16
	
	movdqu xmm0, [ rcx ]	           ; Move second parameter to temporary registar (128 bits)
	movdqu key0, xmm0                  ; Move temporary registar to key0 (128 bits)

	movdqu xmm0, [ rdx ]	           ; Move first parameter to temporary registar (128 bits)
	movdqu nonce, xmm0                 ; Move temporary registar to nonce (128 bits)


	call generate_key_decrypt_schdule  ; Call generate_key_encrypt_schdule to populate the schedule
   
	leave                              ; Restore and return
    ret

initialise_decryption_env ENDP

; Procedure for decrypting a 128 bit block using AES in CTR mode
; Called with function signature void(void*)
decrypt PROC
    push       rbp
    mov        rbp,     rsp
    sub        rsp,     32             ; Shadow Space
    and        spl,     -16            ; Align stack at 16

	movdqu     xmm0,    [ rcx ]        ; Load block onto register

	pxor       xmm0,    key10          ; 
	aesdec     xmm0,    key9
    aesdec     xmm0,    key8
    aesdec     xmm0,    key7
    aesdec     xmm0,    key6
    aesdec     xmm0,    key5
    aesdec     xmm0,    key4
    aesdec     xmm0,    key3
    aesdec     xmm0,    key2
    aesdec     xmm0,    key1
    aesdeclast xmm0,    key0

	movdqu     [ rcx ], xmm0

    leave                              ; Restore stack (rsp) & frame pointer (rbp)
    ret
decrypt ENDP


END
section .text
global iterar_matriz

%define ROWS 12
%define COLS 15
%define ROWLEN COLS + 1

; Entradas:
;   rdi → matriz (char *)
;   rsi → tecla (char)
; Salidas:
;   eax = 0 → acción válida
;   eax = 1 → alguna X destruida
;   eax = -1 → acción inválida
iterar_matriz:
    ; Buscar 'V'
    xor rcx, rcx          ; fila

buscar_fila:
    cmp rcx, ROWS
    jge no_move

    xor rdx, rdx          ; columna
buscar_col:
    cmp rdx, COLS
    jge siguiente_fila

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx
    mov al, [rdi + r9]
    cmp al, 'V'
    je v_encontrada

    inc rdx
    jmp buscar_col

siguiente_fila:
    inc rcx
    jmp buscar_fila

v_encontrada:
    mov r10, r9           ; índice actual de 'V'
    mov r11, rcx          ; fila actual
    mov r12, rdx          ; columna actual

    mov al, sil
    ; cmp al, 'w'
    ; je mover_arriba
    ; cmp al, 's'
    ; je mover_abajo
    cmp al, 'a'
    je mover_izquierda
    cmp al, 'd'
    je mover_derecha
    cmp al, 'k'
    je disparar
    cmp al, 'p'
    je mover_balas
    ; Mover enemigos izq, arr, der, aba: z, x, c, v respectivamente
    cmp al, 'z'
    je enemigos_izquierda
    cmp al, 'x'
    je enemigos_arriba
    cmp al, 'c'
    je enemigos_abajo
    cmp al, 'v'
    je enemigos_derecha
    cmp al, 't'
    je mover_e_izquierda
    cmp al, 'y'
    je mover_e_derecha
    cmp al, 'u'
    je mover_e_arriba
    cmp al, 'i'
    je mover_e_abajo
    

    jmp no_move

; mover_arriba:
;     test r11, r11
;     je no_move
;     dec r11
;     jmp calcular_nueva_pos

; mover_abajo:
;     cmp r11, ROWS - 1
;     jge no_move
;     inc r11
;     jmp calcular_nueva_pos

mover_izquierda:
    test r12, r12
    je no_move
    dec r12
    jmp calcular_nueva_pos

mover_derecha:
    cmp r12, COLS - 1
    jge no_move
    inc r12

calcular_nueva_pos:
    mov r13, r11
    imul r13, ROWLEN
    add r13, r12

    mov al, [rdi + r13]
    cmp al, 'X'
    je eliminar_x

    mov byte [rdi + r13], 'V'
    mov byte [rdi + r10], '0'

    push rdi
    call mover_balas
    pop rdi

    mov eax, 0
    ret

eliminar_x:
    mov byte [rdi + r13], 'V'
    mov byte [rdi + r10], '0'

    ; mover las balas luego de un movimiento válido
    push rdi
    call mover_balas
    pop rdi

    mov eax, 1
    call verificar_enemigos
    ret

disparar:
    test r11, r11
    je no_move

    dec r11
    mov r13, r11
    imul r13, ROWLEN
    add r13, r12

    push rdi
    call mover_balas
    pop rdi
    
    mov al, [rdi + r13]
    cmp al, 'X'
    je disparo_exitoso

    cmp al, '0'
    je colocar_bala

    mov eax, -1
    call verificar_enemigos
    ret

colocar_bala:
    mov byte [rdi + r13], '^'
    mov eax, 0
    call verificar_enemigos
    ret

disparo_exitoso:
    mov byte [rdi + r13], '0'

    mov eax, 1
    call verificar_enemigos
    ret

mover_balas:
    mov rax, 0              ; Valor de retorno por defecto
    mov rcx, 0              ; Fila = 0
bala_fila:
    cmp rcx, ROWS
    jge fin_balas
    mov rdx, 0              ; Columna = 0
bala_col:
    cmp rdx, COLS
    jge sig_bala_fila

    ; Calcular índice: r9 = fila * ROWLEN + col
    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, '^'
    jne siguiente_bala

    ; Si está en la primera fila, eliminar la bala
    test rcx, rcx
    je borrar_bala_fuera

    ; Calcular celda arriba: fila-1
    mov r10, rcx
    dec r10
    imul r11, r10, ROWLEN
    add r11, rdx

    mov bl, [rdi + r11]
    cmp bl, 'X'
    je bala_destruye
    cmp bl, 'E'
    je bala_destruye
    cmp bl, '0'
    je bala_sube

    ; Otro carácter: eliminar bala
    mov byte [rdi + r9], '0'
    jmp siguiente_bala

borrar_bala_fuera:
    mov byte [rdi + r9], '0'
    jmp siguiente_bala

bala_destruye:
    mov byte [rdi + r11], '0'   ; Destruir enemigo
    mov byte [rdi + r9], '0'    ; Eliminar bala
    mov eax, 2                  ; Retornar 2 si destruyó algo
    jmp siguiente_bala

bala_sube:
    mov byte [rdi + r11], '^'   ; Mover bala hacia arriba
    mov byte [rdi + r9], '0'    ; Limpiar posición actual
    jmp siguiente_bala

siguiente_bala:
    inc rdx
    jmp bala_col

sig_bala_fila:
    inc rcx
    jmp bala_fila

fin_balas:
    call verificar_enemigos
    ret

enemigos_izquierda:
    mov rcx, 0               ; fila = 0
fila_enemigos_izq:
    cmp rcx, ROWS
    jge fin_enemigos_izq

    mov rdx, 1               ; columna = 1 (porque no tiene sentido mover desde col 0)
columna_enemigos_izq:
    cmp rdx, COLS
    jge siguiente_fila_enemigos_izq

    mov r8, rcx              ; fila * ROWLEN + col
    imul r9, r8, ROWLEN
    add r9, rdx              ; índice actual

    mov al, [rdi + r9]
    cmp al, 'X'
    jne siguiente_col_enemigos_izq

    ; calcular índice de la celda a la izquierda
    mov r10, r9
    dec r10

    ; revisar si hay espacio libre a la izquierda
    mov bl, [rdi + r10]
    cmp bl, '0'
    jne siguiente_col_enemigos_izq

    ; mover enemigo a la izquierda
    mov byte [rdi + r10], 'X'
    mov byte [rdi + r9], '0'

siguiente_col_enemigos_izq:
    inc rdx
    jmp columna_enemigos_izq

siguiente_fila_enemigos_izq:
    inc rcx
    jmp fila_enemigos_izq

fin_enemigos_izq:
    mov eax, 0
    call verificar_enemigos
    ret

enemigos_arriba:
    mov rcx, 1               ; fila = 1 (no mover fila 0)
fila_arriba:
    cmp rcx, ROWS
    jge fin_enemigos_arriba

    mov rdx, 0               ; columna = 0
col_arriba:
    cmp rdx, COLS
    jge sig_fila_arriba

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'X'
    jne sig_col_arriba

    mov r10, r9
    sub r10, ROWLEN          ; celda arriba

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_arriba

    mov byte [rdi + r10], 'X'
    mov byte [rdi + r9], '0'

sig_col_arriba:
    inc rdx
    jmp col_arriba

sig_fila_arriba:
    inc rcx
    jmp fila_arriba

fin_enemigos_arriba:
    mov eax, 0
    call verificar_enemigos
    ret

enemigos_derecha:
    mov rcx, 0               ; fila = 0
fila_der:
    cmp rcx, ROWS
    jge fin_enemigos_derecha

    mov rdx, COLS-2          ; columna = COLS-2 (mover de derecha a izquierda)
col_der:
    cmp rdx, -1
    jl sig_fila_der

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'X'
    jne sig_col_der

    mov r10, r9
    inc r10                  ; celda derecha

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_der

    mov byte [rdi + r10], 'X'
    mov byte [rdi + r9], '0'

sig_col_der:
    dec rdx
    jmp col_der

sig_fila_der:
    inc rcx
    jmp fila_der

fin_enemigos_derecha:
    mov eax, 0
    call verificar_enemigos
    ret

enemigos_abajo:
    mov rcx, ROWS-2          ; fila = ROWS-2 (de abajo hacia arriba)
fila_aba:
    cmp rcx, -1
    jl fin_enemigos_abajo

    mov rdx, 0               ; columna = 0
col_aba:
    cmp rdx, COLS
    jge sig_fila_aba

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'X'
    jne sig_col_aba

    mov r10, r9
    add r10, ROWLEN          ; celda abajo

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_aba

    mov byte [rdi + r10], 'X'
    mov byte [rdi + r9], '0'

sig_col_aba:
    inc rdx
    jmp col_aba

sig_fila_aba:
    dec rcx
    jmp fila_aba

fin_enemigos_abajo:
    mov eax, 0
    call verificar_enemigos
    ret

mover_e_izquierda:
    mov rcx, 0
fila_e_izq:
    cmp rcx, ROWS
    jge fin_mover_e_izq

    mov rdx, 1
col_e_izq:
    cmp rdx, COLS
    jge sig_fila_e_izq

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'E'
    jne sig_col_e_izq

    mov r10, r9
    dec r10

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_e_izq

    mov byte [rdi + r10], 'E'
    mov byte [rdi + r9], '0'

sig_col_e_izq:
    inc rdx
    jmp col_e_izq

sig_fila_e_izq:
    inc rcx
    jmp fila_e_izq

fin_mover_e_izq:
    mov eax, 0
    call verificar_enemigos
    ret

mover_e_derecha:
    mov rcx, 0
fila_e_der:
    cmp rcx, ROWS
    jge fin_mover_e_derecha

    mov rdx, COLS - 2
col_e_der:
    cmp rdx, -1
    jl sig_fila_e_der

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'E'
    jne sig_col_e_der

    mov r10, r9
    inc r10

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_e_der

    mov byte [rdi + r10], 'E'
    mov byte [rdi + r9], '0'

sig_col_e_der:
    dec rdx
    jmp col_e_der

sig_fila_e_der:
    inc rcx
    jmp fila_e_der

fin_mover_e_derecha:
    mov eax, 0
    call verificar_enemigos
    ret

mover_e_arriba:
    mov rcx, 1
fila_e_arriba:
    cmp rcx, ROWS
    jge fin_mover_e_arriba

    mov rdx, 0
col_e_arriba:
    cmp rdx, COLS
    jge sig_fila_e_arriba

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'E'
    jne sig_col_e_arriba

    mov r10, r9
    sub r10, ROWLEN

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_e_arriba

    mov byte [rdi + r10], 'E'
    mov byte [rdi + r9], '0'

sig_col_e_arriba:
    inc rdx
    jmp col_e_arriba

sig_fila_e_arriba:
    inc rcx
    jmp fila_e_arriba

fin_mover_e_arriba:
    mov eax, 0
    call verificar_enemigos
    ret

mover_e_abajo:
    mov rcx, ROWS - 2
fila_e_abajo:
    cmp rcx, -1
    jl fin_mover_e_abajo

    mov rdx, 0
col_e_abajo:
    cmp rdx, COLS
    jge sig_fila_e_abajo

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'E'
    jne sig_col_e_abajo

    mov r10, r9
    add r10, ROWLEN

    mov bl, [rdi + r10]
    cmp bl, '0'
    jne sig_col_e_abajo

    mov byte [rdi + r10], 'E'
    mov byte [rdi + r9], '0'

sig_col_e_abajo:
    inc rdx
    jmp col_e_abajo

sig_fila_e_abajo:
    dec rcx
    jmp fila_e_abajo

fin_mover_e_abajo:
    mov eax, 0
    call verificar_enemigos
    ret

verificar_enemigos:
    xor rcx, rcx          ; fila

ver_fila:
    cmp rcx, ROWS
    jge enemigos_fuera    ; si no encontró ningún 'X', retorna 2

    xor rdx, rdx          ; columna
ver_col:
    cmp rdx, COLS
    jge sig_fila_ver

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx
    mov al, [rdi + r9]
    cmp al, 'X'
    je enemigos_presentes
    cmp al, 'E'
    je enemigos_presentes

    inc rdx
    jmp ver_col

sig_fila_ver:
    inc rcx
    jmp ver_fila

enemigos_presentes:
    ret                   ; hay enemigos, no cambia eax

enemigos_fuera:
    mov eax, 3            ; no hay enemigos
    ret

no_move:
    mov eax, -1
    call verificar_enemigos
    ret

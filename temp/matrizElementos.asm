section .text
global iterar_matriz

%define ROWS 12
%define COLS 15
%define ROWLEN COLS + 1

iterar_matriz:
    xor rcx, rcx

buscar_fila:
    cmp rcx, ROWS
    jge no_move

    xor rdx, rdx
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
    mov r10, r9
    mov r11, rcx
    mov r12, rdx

    mov al, sil
    cmp al, 'a'
    je mover_izquierda

    cmp al, 'd'
    je mover_derecha

    cmp al, 'k'
    je disparar

    cmp al, 'p'
    je mover_balas_arriba

    cmp al, 'z'
    je mover_x_izquierda

    cmp al, 'x'
    je mover_x_arriba

    cmp al, 'c'
    je mover_x_abajo

    cmp al, 'v'
    je mover_x_derecha

    cmp al, 't'
    je mover_e_izquierda

    cmp al, 'y'
    je mover_e_derecha

    cmp al, 'u'
    je mover_e_arriba
    
    cmp al, 'i'
    je mover_e_abajo
    

    jmp no_move

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

    push r10
    call mover_balas
    pop r10

    mov al, [rdi + r13]
    cmp al, 'X'
    je eliminar_enemigo

    cmp al, 'E'
    je eliminar_enemigo

    mov byte [rdi + r13], 'V'
    mov byte [rdi + r10], '0'

    mov r15, 0
    jmp verificar_enemigos

eliminar_enemigo:
    mov byte [rdi + r13], 'V'
    mov byte [rdi + r10], '0'

    mov r15, 1
    jmp verificar_enemigos

disparar:

    add r11, -1
    mov r13, r11
    imul r13, ROWLEN
    add r13, r12

    call mover_balas
    
    mov al, [rdi + r13]    
    cmp al, 'X'
    je disparo_exitoso

    cmp al, 'E'
    je disparo_exitoso

    cmp al, '0'
    je colocar_bala

    mov eax, -1
    ret

colocar_bala:
    mov byte [rdi + r13], '^'
    cmp r15, 2
    je continuar_colocar_bala
    mov r15, 0
    continuar_colocar_bala:
    jmp verificar_enemigos

disparo_exitoso:
    mov byte [rdi + r13], '0'

    mov r15, 2
    jmp verificar_enemigos

mover_balas_arriba:
    mov r15, 0
    call mover_balas
    jmp verificar_enemigos

mover_balas:
    mov rcx, 0
bala_fila:
    cmp rcx, ROWS
    jge fin_balas
    mov rdx, 0
bala_col:
    cmp rdx, COLS
    jge sig_bala_fila

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, '^'
    jne siguiente_bala

    test rcx, rcx
    je borrar_bala_fuera

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

    mov byte [rdi + r9], '0'
    jmp siguiente_bala

borrar_bala_fuera:
    mov byte [rdi + r9], '0'
    jmp siguiente_bala

bala_destruye:
    mov byte [rdi + r11], '0'
    mov byte [rdi + r9], '0'
    mov r15, 2
    jmp siguiente_bala

bala_sube:
    mov byte [rdi + r11], '^'
    mov byte [rdi + r9], '0'
    jmp siguiente_bala

siguiente_bala:
    inc rdx
    jmp bala_col

sig_bala_fila:
    inc rcx
    jmp bala_fila

fin_balas:
    ret

mover_x_izquierda:
    mov r15, 0
    mov rcx, 0
fila_enemigos_izq:
    cmp rcx, ROWS
    jge fin_enemigos_izq

    mov rdx, 1
columna_enemigos_izq:
    cmp rdx, COLS
    jge siguiente_fila_enemigos_izq

    mov r8, rcx
    imul r9, r8, ROWLEN
    add r9, rdx

    mov al, [rdi + r9]
    cmp al, 'X'
    jne siguiente_col_enemigos_izq

    mov r10, r9
    dec r10

    mov bl, [rdi + r10]
    cmp bl, '^'
    jne continuar_x_izq_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_x_izq_bala:
    cmp bl, 'V'
    jne continuar_x_izq
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_x_izq:
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
    call mover_balas
    jmp verificar_enemigos

mover_x_arriba:
    mov r15, 0
    mov rcx, 1
fila_arriba:
    cmp rcx, ROWS
    jge fin_mover_x_arriba

    mov rdx, 0
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
    sub r10, ROWLEN

    mov bl, [rdi + r10]
    cmp bl, '^'
    jne continuar_x_arr_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_x_arr_bala:
    cmp bl, 'V'
    jne continuar_x_arr
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_x_arr:
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

fin_mover_x_arriba:
    call mover_balas
    jmp verificar_enemigos

mover_x_derecha:
    mov r15, 0
    mov rcx, 0
fila_der:
    cmp rcx, ROWS
    jge fin_mover_x_derecha

    mov rdx, COLS-2
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
    inc r10

    mov bl, [rdi + r10]
    cmp bl, '^'
    jne continuar_x_der_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_x_der_bala:
    cmp bl, 'V'
    jne continuar_x_der
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_x_der:
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

fin_mover_x_derecha:
    call mover_balas
    jmp verificar_enemigos

mover_x_abajo:
    mov r15, 0
    mov rcx, ROWS-2
fila_aba:
    cmp rcx, -1
    jl fin_enemigos_abajo

    mov rdx, 0
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
    add r10, ROWLEN

    mov bl, [rdi + r10]
    cmp bl, '^'
    jne continuar_x_aba_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_x_aba_bala:
    cmp bl, 'V'
    jne continuar_x_aba
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_x_aba:
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
    call mover_balas
    jmp verificar_enemigos

mover_e_izquierda:
    mov r15, 0
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
    cmp bl, '^'
    jne continuar_e_izq_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_e_izq_bala:
    cmp bl, 'V'
    jne continuar_e_izq
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_e_izq:
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
    call mover_balas
    jmp verificar_enemigos

mover_e_derecha:
    mov r15, 0
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
    cmp bl, '^'
    jne continuar_e_der_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_e_der_bala:
    cmp bl, 'V'
    jne continuar_e_der
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_e_der:
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
    call mover_balas
    jmp verificar_enemigos

mover_e_arriba:
    mov r15, 0
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
    cmp bl, '^'
    jne continuar_e_arr_bala
    mov r15, 2
    mov byte [rdi + r10], '0'
    mov byte [rdi + r9], '0'
    continuar_e_arr_bala:
    cmp bl, 'V'
    jne continuar_e_arr
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_e_arr:
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
    call mover_balas
    jmp verificar_enemigos

mover_e_abajo:
    mov r15, 0
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
    cmp bl, 'V'
    jne continuar_e_aba
    mov r15, 1
    mov byte [rdi + r9], '0'
    continuar_e_aba:
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
    call mover_balas
    call verificar_enemigos
    mov rax, r15

verificar_enemigos:
    xor rcx, rcx

ver_fila:
    cmp rcx, ROWS
    jge enemigos_fuera

    xor rdx, rdx
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
    mov eax, r15d
    ret

enemigos_fuera:
    mov eax, 3
    ret

no_move:
    mov eax, -1
    ret
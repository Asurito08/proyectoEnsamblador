section .text
global iterar_matriz

%define ROWS 7
%define COLS 9
%define ROWLEN 10

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
    je encontrada_v

    inc rdx
    jmp buscar_col

siguiente_fila:
    inc rcx
    jmp buscar_fila

encontrada_v:
    mov r10, r9           ; índice actual de 'V'
    mov r11, rcx          ; fila actual
    mov r12, rdx          ; columna actual

    mov al, sil
    cmp al, 'w'
    je mover_arriba
    cmp al, 's'
    je mover_abajo
    cmp al, 'a'
    je mover_izquierda
    cmp al, 'd'
    je mover_derecha
    cmp al, 'k'
    je disparar_arriba
    cmp al, 'p'
    je mover_balas

    jmp no_move

mover_arriba:
    test r11, r11
    je no_move
    dec r11
    jmp calcular_nueva_pos

mover_abajo:
    cmp r11, ROWS - 1
    jge no_move
    inc r11
    jmp calcular_nueva_pos

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

    ; mover las balas luego de un movimiento válido
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
    ret

disparar_arriba:
    test r11, r11
    je no_move

    dec r11
    mov r13, r11
    imul r13, ROWLEN
    add r13, r12

    call mover_balas

    mov al, [rdi + r13]
    cmp al, 'X'
    je disparo_exitoso

    cmp al, '0'
    je colocar_bala

    mov eax, -1
    ret

colocar_bala:
    mov byte [rdi + r13], '^'
    mov eax, 0
    ret

disparo_exitoso:
    mov byte [rdi + r13], '0'

    ; mover balas tras disparar
    push rdi
    pop rdi

    mov eax, 1
    ret

mover_balas:
    mov rax, 0
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

    ; Si está en la primera fila, eliminar la bala
    test rcx, rcx
    je borrar_bala_fuera

    ; Calcular celda arriba
    mov r10, rcx
    dec r10
    imul r11, r10, ROWLEN
    add r11, rdx

    mov bl, [rdi + r11]
    cmp bl, 'X'
    je bala_destruye_x
    cmp bl, '0'
    je bala_sube

    ; Otro caracter, eliminar bala
    mov byte [rdi + r9], '0'
    jmp siguiente_bala

borrar_bala_fuera:
    mov byte [rdi + r9], '0'
    jmp siguiente_bala

bala_destruye_x:
    mov byte [rdi + r11], '0'
    mov byte [rdi + r9], '0'
    mov eax, 1
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

no_move:
    mov eax, -1
    ret

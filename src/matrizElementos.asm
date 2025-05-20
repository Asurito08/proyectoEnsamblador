section .data
    fileName db "saves/nivel.txt", 0
    rows equ 7
    cols equ 9
    totalBytes equ rows * (cols + 1) ; +1 Para almacenar el cambio de línea directamente
    clearScreen db 27, '[', '2', 'J', 27, '[', 'H'
    clearLen equ $ - clearScreen
    loseMsg db "You lost", 10
    loseLen equ $ - loseMsg

section .bss
    key resb 1
    matrix resb totalBytes
    index resb 2 ; index[0] = row, index[1] = col

section .text
    global _start

%define STDIN 0
%define STDOUT 1

_start:
    ; Abrir archivo
    mov rax, 2
    mov rdi, fileName
    xor rsi, rsi
    xor rdx, rdx
    syscall
    mov r12, rax

    ; Leer archivo
    mov rax, 0
    mov rdi, r12
    mov rsi, matrix
    mov rdx, totalBytes
    syscall

    ; Cerrar Archivo
    mov rax, 3
    mov rdi, r12
    syscall

mainLoop:
    ; Limpiar pantalla
    mov rax, 1
    mov rdi, STDOUT
    mov rsi, clearScreen
    mov rdx, clearLen
    syscall

    ; Imprimir matriz
    mov rax, 1
    mov rdi, STDOUT
    mov rsi, matrix
    mov rdx, totalBytes
    syscall

    ; Leer tecla
    mov rax, 0
    mov rdi, STDIN
    mov rsi, key
    mov rdx, 1
    syscall
    cmp rax, 0

    ; Comparar la tecla
    mov al, [key]
    cmp al, 'q'
    je gameQuit
    cmp al, 'a'
    je moveLeft
    cmp al, 'd'
    je moveRight
    cmp al, 'w'
    je moveUp
    cmp al, 's'
    je moveDown

    jmp mainLoop

; Almacenar la posición del personaje en row y col
findV:
    xor al, al
    mov [index], al
searchRow:
    cmp byte [index], rows
    xor al, al
    mov [index + 1], al
searchCol:
    cmp byte [index + 1], cols
    jge nextRow

    ; Calcular el índice total
    movzx rax, byte [index]
    imul rdx, rax, cols + 1
    movzx rax, byte [index + 1]
    add rdx, rax

    ; Verificar si es la V
    mov al, [matrix + rdx]
    cmp al, 'V'
    je foundV

    inc byte [index + 1]
    jmp searchCol

nextRow:
    inc byte [index]
    jmp searchRow

foundV:
    ret

; Moverse a la izqquierda si está en rango
moveLeft:
    call findV
    cmp byte [index + 1], 0
    je mainLoop

    ; Conseguir el índice total actual
    movzx rax, byte [index]
    imul rdx, rax, cols + 1
    movzx rax, byte [index + 1]
    add rdx, rax

    mov rbx, matrix
    add rbx, rdx

    ; Verificar si hay una X
    mov al, [rbx - 1]
    cmp al, 'X'
    je youLost

    ; Mover X a la izquierda
    mov cl, [rbx]
    mov al, [rbx - 1]
    mov [rbx], al
    mov [rbx - 1], cl
    jmp mainLoop

; Mover a la derecha si está en rango
moveRight:
    call findV
    mov al, [index + 1]
    cmp al, cols - 1
    jae mainLoop

    movzx rax, byte [index]
    imul rdx, rax, cols + 1
    movzx rax, byte [index + 1]
    add rdx, rax

    mov rbx, matrix
    add rbx, rdx

    ; Verificar si hay una X
    mov al, [rbx + 1]
    cmp al, 'X'
    je youLost

    ; Mover a la derecha
    mov cl, [rbx]
    mov al, [rbx + 1]
    mov [rbx], al
    mov [rbx + 1], cl
    jmp mainLoop

; Moverse hacia arriba si está en rango
moveUp:
    call findV
    cmp byte [index], 0
    je mainLoop

    movzx rax, byte [index]
    imul rdx, rax, cols + 1
    movzx rax, byte [index + 1]
    add rdx, rax

    mov rbx, matrix
    add rbx, rdx

    ; Calcular el índice total
    mov al, [rbx - (cols + 1)]
    cmp al, 'X'
    je youLost

    mov cl, [rbx]
    mov [rbx], al
    mov [rbx - (cols + 1)], cl
    jmp mainLoop

; Moverse abajo si está en rango
moveDown:
    call findV
    mov al, [index]
    cmp al, rows - 1
    jae mainLoop

    movzx rax, byte [index]
    imul rdx, rax, cols + 1
    movzx rax, byte [index + 1]
    add rdx, rax

    mov rbx, matrix
    add rbx, rdx

    ; Calcular el índice total del elemento debajo
    mov al, [rbx + (cols + 1)]
    cmp al, 'X'
    je youLost

    mov cl, [rbx]
    mov [rbx], al
    mov [rbx + (cols + 1)], cl
    jmp mainLoop

youLost:
    mov rax, 1
    mov rdi, STDOUT
    mov rsi, loseMsg
    mov rdx, loseLen
    syscall

    mov rax, 60
    mov rdi, 1
    syscall

gameQuit:
    mov rax, 60
    xor rdi, rdi
    syscall


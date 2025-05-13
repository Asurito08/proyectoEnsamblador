section .data
    ; Nombre del archivo
    filename db "saves/nivel.txt", 0
    ; Datos relacionados a la matriz
    rows equ 7
    cols equ 9
    totalBytes equ rows * (cols + 1)

    ; Datos relacionados a la secuencia ansi para limpiar la pantalla
    clearScreen db 27, '[', '2', 'J', 27, '[', 'H'
    clearLen equ $ - clearScreen

    ; Indicaciones para esperar
    timespec:
        dq 0
        dq 100000000

section .bss
    matriz resb totalBytes

section .text
    global _start

_start:
    ; Índice para mantener el bucle
    xor r8, r8

    ; Abrir archivo
    mov rax, 2
    mov rdi, filename
    xor rsi, rsi
    xor rdx, rdx
    syscall
    mov r12, rax

    ; Leer archivo
    mov rax, 0
    mov rdi, r12
    mov rsi, matriz
    mov rdx, totalBytes
    syscall

    ; Cerrar archivo
    mov rax, 3
    mov rdi, r12
    syscall

    buclePrincipal:
        ; Limpiar pantalla con ANSI
        mov rax, 1
        mov rdi, 1
        mov rsi, clearScreen
        mov rdx, clearLen
        syscall

        ; Imprimir matriz
        mov rax, 1
        mov rdi, 1
        mov rsi, matriz
        mov rdx, totalBytes
        syscall

salir:
    mov rax, 60
    xor rdi, rdi
    syscall
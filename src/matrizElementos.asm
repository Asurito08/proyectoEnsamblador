section .data
    filename db "saves/nivel.txt", 0
    rows equ 7
    cols equ 9
    totalBytes equ rows * (cols + 1)
    clearScreen db 27, '[', '2', 'J', 27, '[', 'H'
    clearLen equ $ - clearScreen

section .bss
    tecla resb 1
    matriz resb totalBytes

section .text
    global _start

%define STDIN 0
%define STDOUT 1

_start:
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

bucle:
    ; Limpiar pantalla
    mov rax, 1
    mov rdi, STDOUT
    mov rsi, clearScreen
    mov rdx, clearLen
    syscall

    ; Imprimir matriz
    mov rax, 1
    mov rdi, STDOUT
    mov rsi, matriz
    mov rdx, totalBytes
    syscall

    ; Leer una tecla
    mov rax, 0
    mov rdi, STDIN
    mov rsi, tecla
    mov rdx, 1
    syscall
    cmp rax, 0
    jl error

    ; Verificar si es 'q'
    mov al, [tecla]
    cmp al, 'q'
    je salir

    ; Verificar si es 'a'
    mov al, [tecla]
    cmp al, 'a'
    je teclaA

    jmp bucle

teclaA:
    

salir:
    mov rax, 60
    xor rdi, rdi
    syscall

error:
    mov rax, 60
    mov rdi, 1
    syscall

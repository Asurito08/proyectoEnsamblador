#!/bin/bash

echo "🔧 Iniciando configuración del entorno para el proyecto de Lenguaje Ensamblador..."

# Paso 1: Actualizar paquetes
echo "📦 Actualizando sistema..."
sudo apt update && sudo apt upgrade -y

# Paso 2: Instalar NASM
echo "📥 Instalando NASM..."
sudo apt install -y nasm
nasm -v || { echo "❌ Error instalando NASM"; exit 1; }

# Paso 3: Instalar GCC (build-essential)
echo "📥 Instalando GCC y herramientas esenciales..."
sudo apt install -y build-essential
gcc --version || { echo "❌ Error instalando GCC"; exit 1; }

# Paso 4: Instalar VS Code
echo "📥 Instalando Visual Studio Code..."
sudo apt install -y wget gpg
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
sudo sh -c 'echo "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list'
sudo apt update
sudo apt install -y code
rm packages.microsoft.gpg

# Paso 5: Instalar GTK4
echo "📥 Instalando GTK4..."
sudo apt install -y libgtk-4-dev
pkg-config --modversion gtk4 || { echo "❌ Error instalando GTK4"; exit 1; }

# Paso 6: Clonar el repositorio
echo "🔗 Clonando repositorio del proyecto..."
if [ -d "proyectoEnsamblador" ]; then
    echo "⚠️ Ya existe una carpeta llamada 'proyectoEnsamblador', omitiendo clonación."
else
    git clone https://github.com/Asurito08/proyectoEnsamblador.git || { echo "❌ Error al clonar repositorio"; exit 1; }
fi

# Paso 7: Compilar y correr el programa
echo "🛠️ Compilando y ejecutando el proyecto..."
cd proyectoEnsamblador
make run

echo "✅ Configuración completada y proyecto ejecutado."

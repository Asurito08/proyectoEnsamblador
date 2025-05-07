# proyectoEnsamblador
Repositorio para subir y compartir los avances en el proyecto del curso I - S - 2025 - RRF - Lenguaje Ensamblador - 002

Fabricio Padilla Madrigal – C35837 
Wilson Chavarría Miranda – C22114 
Eliot Masís Flores – C24621

Guía de configuración del entorno virtual para poder correr el proyecto.

Si aun no tienes un entorno de desarrollo linux configurado sigue los siguientes pasos para poder configurar desde cero una máquina virtual configurada con Ubuntu, si ya lo tienes salta al paso 2.

Paso 1 Configurar tu máquina virtual:

-Ingresamos a la página de Broadcom.    
-Nos vamos a la sección de soporte, luego VMware, productos y buscamos VMware Workstation Pro.
-Descargamos el instalador y lo ejecutamos.
-Seguimos las instrucciones del instalador y finalizamos la instalación del Worstation.
-Descargamos la imagen de linux que utilizaremos, en este caso una imagen de Ubuntu.
-Ingresamos a la pagina de Ubuntu, luego a Download Ubuntu y seleccionamos Download Ubuntu Desktop.
-Con la imagen ya descargada abrimos Vmware Workstation, nos vamos a la pestañan File, presionamos New Virtual Machine.
-Seleccionamos la configuración típica, presionamos Next y seguimos los pasos del instalador, las caracteríscas de nuestra VM las vamos eligiendo sobre el proceso de instalacion.
-Como recomendación seleccionar un mínimo de 6 gigas de RAM y 8 procesadores.

Paso 2 Instalar Nasm:

-Para instalar Nasm que es el ensamblador que vamos a utilizar para nuestro código, digita los siguientes comandos en la terminal.

      sudo pat update
      sudo apt install nasm
           
-Para verificar que el Nasm se instalara correctamente se digita el comando.

      nasm -v
      
-Debería mostrar la última versión de nasm, si esto no es así vuelve a repetir el paso 2.
      
Paso 3 Instalar GCC:

-Normalmente el GCC viene instalado con linux, pero igual vamos a mostrar como instalarlo por cualquier inconveniente.
-Para verificar si está instalado digita el comando 

             gcc --version

 -Si no se muestra la versión entonces debes instalarlo con los comandos
 
            sudo apt update
            sudo apt install build-essential
            
-Volvemos a verificar si está instalado con el primer comando y si todo salió bien nos debería mostrar la última version.

Paso 4 Instalar VScode:

-Instalaremos VScode como editor de código para el proyecto.
-Para instalarlo digitamos los siguientes comandos

            sudo apt update
            
            sudo apt install wget gpg
            
            wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg
            --dearmor > packages.microsoft.gpg
            sudo install -o root -g root -m 644 packages.microsoft.gpg
            /etc/apt/trusted.gpg.d/
            
            sudo sh -c 'echo "deb [arch=amd64]
            https://packages.microsoft.com/repos/vscode stable main" >
            /etc/apt/sources.list.d/vscode.list'
            
            sudo apt update
            sudo apt install code
            
-Con esto deberías poder abrirlo utilizando el comando

            code

Paso 5 Instalar GTK4:

-Instalar la bibiloteca gráfica con la que vamos a trabajar, en este caso gtk4
-Para instalarla digitamos los comandos:

            sudo apt update
            
            sudo apt install libgtk-4-dev
            
-Con el siguiente comando verificas si se instaló correctamente

            pkg-config --modversion gtk4.
            
Paso 6 Clonar el Repositorio:

-Dentro de la carpeta en la que quieras guardar tu repositorio localmente digita el siguiente comando

              git clone https://github.com/Asurito08/proyectoEnsamblador.git
              
-Te pedira tu usuario y tu contraseña de github, esto por ser un repositorio privado, luego de digitar esto se clonara el repositorio virtual en tu computadora.
    
Paso 7 Correr el Programa: 

-Dentro de la carpeta del programa llamada proyectoEnsamblador, digita el comando

              make run
            
-Esto debería compilarte el programa, creando un ejecutable y  luego ejecutar el archivo que acaba de generar.

Con esto deberías poder correr el programa completo y sin problemas.

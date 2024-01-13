<h1 align="center">Servidor y Cliente de Chat en C</h1>

---

## Descripción

Este repositorio contiene un servidor y cliente de chat implementados en C utilizando sockets. El servidor es capaz de manejar múltiples clientes y distribuir mensajes entre ellos.

## Contenido

- [Requisitos](#requisitos)
- [Compilación](#compilación)
- [Uso](#uso)
- [Funcionalidades](#funcionalidades)
- [Configuración Personalizada](#configuración-personalizada)
- [Contribuciones](#contribuciones)
- [Licencia](#licencia)

## Requisitos

- Compilador de C (por ejemplo, GCC para Windows o Linux)
- Biblioteca Winsock para Windows

## Compilación

Utiliza el siguiente comando para compilar el servidor y el cliente:

```bash
make main
```

Esto generará dos ejecutables: socketSV.exe para el servidor y socketCL.exe para el cliente.

Uso
Ejecuta el servidor en una terminal:
```
./socketSV.exe
```
Ejecuta el cliente en otra terminal y sigue las instrucciones para ingresar la IP del servidor y tu nombre de usuario:
```
./socketCL.exe
```

## ¡Comienza a chatear!

### Funcionalidades

El servidor puede manejar múltiples clientes simultáneamente.

El cliente puede enviar mensajes al servidor y recibir mensajes de otros clientes.

### Configuración Personalizada

Puedes personalizar el código según tus necesidades. Modifica las extensiones de archivo, la cantidad máxima de clientes o cualquier otra configuración en los archivos socketSV.c y socketCL.c.

### Contribuciones
Las contribuciones son bienvenidas. Si encuentras algún problema o tienes sugerencias de mejora, por favor, abre un problema o crea una solicitud de extracción.

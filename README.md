# 🤖 Robot Autónomo con RISC-V — Localizador de Objetivos Visuales

Proyecto de robot móvil autónomo basado en un procesador **RISC-V**, diseñado para operar en **espacios cerrados**, detectar **marcadores visuales de colores**, **almacenar trayectorias**, y desplazarse entre objetivos según rutas aprendidas.

---

## 🧠 Resumen del Proyecto

El sistema permite que un robot se desplace de forma autónoma siguiendo objetivos visuales (colores predefinidos), registrando su trayectoria y permitiendo navegar entre esos puntos de forma controlada.

---

## 📋 Requerimientos Funcionales

- ✅ Detección de marcadores visuales por color (ej. rojo, azul, verde).
- ✅ Trazado de rutas según la secuencia de colores detectados.
- ✅ Almacenamiento y recuperación de trayectorias.
- ✅ Navegación autónoma hacia objetivos registrados.

---

## 📥 Entradas del Sistema

- 🎥 **Señal de cámara** (para detección visual).
- 📡 **Sensores de proximidad** (evitar obstáculos).
- ⏱️ **Reloj del sistema** (sincronización).

## 📤 Salidas del Sistema

- 🔁 **Señales de control de motores** (adelante, atrás, giro).
- 💾 **Registro de datos** (posición y color detectado).
- 💡 **LEDs de estado** (visualización de modos/estados).

---

## 🧩 Arquitectura Modular

### 🎨 Módulo de Detección de Color
- **Función:** Procesa señales de video para identificar colores específicos.
- **Entrada:** Señal de cámara.
- **Salida:** `color_detectado` (ej: 00 = ninguno, 01 = rojo, 10 = azul...).

### 🔁 Módulo de Control de Movimiento
- **Función:** Máquina de estados finitos (FSM) para determinar la dirección del robot.
- **Entrada:** `color_detectado` + sensores de proximidad.
- **Salida:** Señales PWM a los motores.

### 🧠 Módulo de Memoria / Trayectoria
- **Función:** Guarda información sobre las trayectorias recorridas.
- **Entrada:** Datos de ubicación + color.
- **Salida:** Trayectoria para navegación posterior.

### 🧾 Unidad de Control General
- **Función:** Coordina todos los módulos.
- **Procesador:** Basado en arquitectura **RISC-V**.


## 🚀 Estado del Proyecto

- [x] Diseño conceptual ✅  
- [ ] Implementación de módulos individuales  
- [ ] Integración de subsistemas  
- [ ] Pruebas en entorno controlado  
- [ ] Optimización y documentación final  

---

## 📌 Créditos y Autores

Proyecto desarrollado por [Tu nombre o grupo de trabajo].  
Contribuciones, sugerencias y mejoras son bienvenidas.

---

## 📄 Licencia

Este proyecto está bajo la licencia [MIT/GPL/u otra].


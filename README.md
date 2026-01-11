# Blink ⚡

**Blink** es un experimento educativo y técnico para construir, desde cero, un **runtime de UI inspirado en React Native**, usando:

- **C++** como motor
- **QuickJS** como runtime de JavaScript
- Un **árbol de vistas propio**
- Un **renderer desacoplado** (actualmente por consola)

El objetivo de Blink es **entender cómo funcionan realmente los frameworks modernos por dentro**.

---

## Objetivos del proyecto

- Aprender cómo se integra un motor JS con C++
- Construir un árbol de vistas jerárquico
- Exponer una API JS mínima (`Blink.*`)
- Renderizar el árbol (debug / consola)
- Preparar el terreno para diff, layout y render real
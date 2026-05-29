# Sistema de Monitoramento Orbital e Desvio de Detritos

Protótipo de sistema embarcado desenvolvido para detecção de proximidade de objetos e execução de manobras automáticas de desvio utilizando a plataforma Arduino.

## Arquitetura do Hardware

O circuito foi montado utilizando os seguintes componentes:
* **Microcontrolador:** Arduino Uno R3
* **Sensor de Proximidade:** Ultrassônico HC-SR04
* **Atuador:** Servomotor Padrão (Simulação de Propulsor)
* **Indicador Visual:** LED Vermelho (Alerta) com resistor de 220Ω
* **Telemetria Local:** Display LCD 16x2 e Potenciômetro de 10kΩ (Controle de Contraste)

### Mapeamento de Pinos (I/O)

| Componente | Pino Arduino | Tipo | Função |
| :--- | :--- | :--- | :--- |
| **HC-SR04 (Trig)** | 9 | Output | Gatilho do pulso ultrassônico |
| **HC-SR04 (Echo)** | 10 | Input | Leitura do tempo de retorno do eco |
| **Servomotor** | 11 | Output (PWM) | Controle angular do propulsor |
| **LED Alerta** | 13 | Output | Sinalização visual de proximidade |
| **LCD RS** | 12 | Output | Sinal de controle do display |
| **LCD Enable** | 7 | Output | Sinal de sincronismo do display |
| **LCD D4-D7** | 5, 4, 3, 2 | Output | Barramento de dados de 4 bits |

---

## Máquina de Estados do Software

O firmware opera sob uma lógica não-bloqueante com taxa de amostragem fixa em 500ms. O sistema chaveia entre três estados operacionais distintos de acordo com a distância lida:

1. **Estado Nominal (> 100 cm):**
   * Display exibe `ORBITA SEGURA`.
   * Servomotor mantido em 0°.
   * LED indicador desativado.

2. **Estado De Alerta (30 cm a 100 cm):**
   * Display exibe `ALERTA: DETRITO`.
   * Servomotor mantido em 0°.
   * LED indicador pisca na frequência de amostragem.

3. **Estado Crítico (< 30 cm):**
   * Display exibe `CRITICO! OBJETO`.
   * Servomotor deflete imediatamente para 90° para simular guinada de desvio.
   * LED indicador ativado continuamente.

### Filtro de Tolerância a Falhas

Caso o sensor ultrassônico retorne uma leitura inválida (ruído elétrico, ausência de eco ou valores fora do range de 0 a 400 cm), o software descarta a amostra anômala, exibe a mensagem `ERR: SENSOR FALH` na segunda linha do LCD e mantém o último estado de distância válido registrado para evitar oscilações e ações erráticas dos atuadores.

---

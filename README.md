## Plataformas de Hardware para Internet das Coisas
### Docente: Luiz de França Afonso Ferreira Filho 
### Discentes: 
- Davi Vieira de Carvalho lima
- Leandro Antony Batista Lemos
- Mariana Jamile dos Santos Ferreira
- Vinicius Costa Soares

# ESP32: medidor de distância com sensor ultrassônico HC-SR04

Projeto de laboratório para ESP32 que combina **saída digital (pulso de disparo)**, **medição de pulso (`pulseIn`)** e **comunicação serial** em um único circuito. O ESP32 dispara o sensor **HC-SR04**, mede o tempo de retorno do eco e converte esse tempo em distância, exibindo o resultado no monitor serial. A distância máxima de detecção é configurável: objetos além do limite resultam em leitura `0`.

## Sumário CORRIGIR SUMÁRIO

- [Funcionalidades](#funcionalidades)
- [Materiais](#materiais)
- [Mapeamento de pinos](#mapeamento-de-pinos)
- [Esquema de ligação](#esquema-de-ligação)
- [Diagrama do circuito](#diagrama-do-circuito)
- [Como funciona](#como-funciona)
- [Fotos](#fotos)
- [Como compilar e gravar](#como-compilar-e-gravar)
- [Saída serial](#saída-serial)
- [Problemas enfrentados](#problemas-enfrentados)
- [Parâmetros configuráveis](#parâmetros-configuráveis)
- [Estrutura do repositório](#estrutura-do-repositório)

## Funcionalidades

- Disparo do sensor com pulso de 10 µs no pino TRIG.
- Medição da duração do eco no pino ECHO com `pulseIn()`.
- Conversão do tempo em distância (cm) usando a velocidade do som.
- Distância máxima de detecção ajustável por uma única constante (`DISTANCIA_MAXIMA`).
- Retorno `0` para objetos fora do alcance configurado.
- Monitoramento das medições pela porta serial (115200 baud).

## Materiais

| Qtd | Componente | Observação |
|-----|------------|------------|
| 1 | Placa ESP32 (DevKit) | |
| 1 | Sensor ultrassônico HC-SR04 | Faixa de 2 cm a 400 cm (datasheet) |
| 1 | Resistor 1,1 kΩ | Divisor de tensão no ECHO |
| 1 | Resistor 2 kΩ | Divisor de tensão no ECHO |
| — | Protoboard e jumpers | Quanto for necessário |

## Mapeamento de pinos

| Função | GPIO | Tipo | Observação |
|--------|------|------|------------|
| TRIG | 27 | Saída digital | Pulso de 10 µs |
| ECHO | 26 | Entrada digital | Via divisor de tensão (5 V → ~3,2 V) |
| VCC do sensor | 5V | Alimentação | O HC-SR04 opera em 5 V |
| GND | GND | Terra comum | Sensor, divisor e ESP32 |

## Esquema de ligação

- **VCC:** pino VCC do HC-SR04 no 5V do ESP32.
- **GND:** pino GND do HC-SR04 no GND do ESP32.
- **TRIG:** pino TRIG do HC-SR04 direto no GPIO 27.
- **ECHO:** o ECHO sai em 5 V, mas os GPIOs do ESP32 suportam 3,3 V. Por isso ele passa por um **divisor de tensão**:

```
ECHO ──[ 1,1 kΩ ]──┬──[ 2 kΩ ]── GND
                   │
                 GPIO 26
```

$$V_{out} = 5\text{V} \cdot \frac{2000}{1100 + 2000} \approx 3{,}23\,\text{V}$$

> A ordem importa: o resistor de 2 kΩ fica do lado do GND.

### Diagrama do circuito
(ADICIONAR IMAGEM)

(Wokwi)

## Como funciona

1. Na inicialização, o firmware configura a serial (115200 baud), o pino TRIG como saída e o pino ECHO como entrada.
2. A cada ciclo do `loop()` (~500 ms):
   1. Coloca o TRIG em LOW por 2 µs para garantir um nível limpo.
   2. Envia um pulso HIGH de 10 µs no TRIG.
   3. Mede com `pulseIn()` quanto tempo o ECHO permanece em HIGH (tempo de ida e volta do som).
   4. Calcula a distância: o som percorre ida e volta, então o tempo é dividido por 2.
   5. Se a distância passar de `DISTANCIA_MAXIMA`, o valor é substituído por `0`.
   6. Imprime o resultado na serial.

$$d = \frac{v \cdot t}{2}, \quad v \approx 0{,}0343\ \text{cm/µs}$$

| Situação | Saída |
|----------|-------|
| Objeto dentro do alcance configurado | Distância medida em cm |
| Objeto além de `DISTANCIA_MAXIMA` | `0` |
| Nenhum eco recebido | `0` (o `pulseIn` devolve 0 após o timeout padrão de 1 s) |

### Fotos (ADICIONAR)

- Circuito montado e funcionando

<img src="./images/circuito_montado.jpg" alt="" width="300">

- Medições no monitor serial (objeto próximo)

<img src="./images/medicao_proxima.jpg" alt="" width="300">

- Medições no monitor serial (objeto fora do alcance: leitura 0)

<img src="./images/medicao_fora_alcance.jpg" alt="" width="300">

## Como compilar e gravar

### Arduino IDE

1. Instale o pacote de placas **esp32 by Espressif Systems** (Gerenciador de Placas).
2. Selecione a placa **ESP32 Dev Module** (ou a que corresponde à sua).
3. Abra o arquivo do projeto, compile e faça o upload.
4. Abra o **Monitor Serial** em **115200 baud**.

## Saída serial

Configure o monitor serial para **115200 baud**. Cada ciclo imprime uma linha com a distância medida:

```
Distância: 30.12 cm
Distância: 30.05 cm
Distância: 0.00 cm
```

## Problemas enfrentados
No projeto realizado, não se possuía um esp32 com saída de 5V, portanto, foi utilizado a saída de 3,3V.
O funcionamento ocorreu normalmente, exceto quando a medição exige longas distâncias, visto que o sensor não estava trabalhando com a Tensão ideal que ele deveria operar.
A medição mais precisa que foi obtida com 3,3V foi de até aproximadamente 2m de distância.

## Parâmetros configuráveis

| Constante | Valor | Descrição |
|-----------|-------|-----------|
| `TRIG` | 27 | Pino de disparo do sensor |
| `ECHO` | 26 | Pino de leitura do eco |
| `DISTANCIA_MAXIMA` | 100.0 | Distância máxima de detecção (cm); além dela a leitura é 0 |

Para mudar o alcance, edite a constante:

```cpp
const float DISTANCIA_MAXIMA = 100.0; // detecta até 100 cm
```

## Estrutura do repositório

```
.
├── images
│    └── [...]
├── src
│    └── sensor_ultrassom.ino
└── README.md

```

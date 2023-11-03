# C
dodajac nowy plik
- tworzycie naglowek .h zawierajacy forward declaration
- tworzycie naglowek .c zawierajacy implementacje
- do main/CMakeLists.txt w pierwszej linii dopisujecie nowo stworzony plik .c
- w #include ... includujecie **TYLKO** .h

jesli plytka sie restartuje to prawdopodobnie polecial core (segfault)

# Pinout
to jest pinout ktorego ja uzywam, mozna go zmieniac przez config

uwaga na pin przycisku, pin musi obslugiwac pullup

https://asciiflow.com

        ┌────────────────┐
        │EN           D23│
        │VP              │
        │VN           D22├──────┐
        │D34          -  │      │         ┌────────────┐
        │D35          -  │      │         │    OLED    │
        │D32          D21├──────┼─────────┤SDA         │
        │D33             │      │         │            │
        │             D19│      └─────────┤SCL         │
        │D25             │                │            │
        │             D18│         ┌──────┤VCC         │
        │D26             │         │      │            │
        │             D5 │      ┌──┼──────┤GND         │
   ┌────┤D27          TX2│      │  │      └────────────┘
   │    │             RX2│      │  │
   │    │D14          D4 │      │  │
   │    │                │      │  │      ┌─────────┐
   │    │D12          D2 │      ├──┼──────┤-        │
   │    │                │      │  │      │ BUTTON  │
   │    │D13          D15│      │  │   ┌──┤-        │
   │    │                │      │  │   │  └─────────┘
   │    │GND          GND├──────┘  │   │
   │    │                │         │   │
   │    │VIN          3V3├─────────┘   │
   │    └────────────────┘             │
   │                                   │
   │                                   │
   └───────────────────────────────────┘

# OLED
https://esp32tutorials.com/oled-esp32-esp-idf-tutorial/

# WIFI
siec musi byc na 2.4 GHz

jesli pingujecie swoj komputer trzeba wylaczyc firewalla



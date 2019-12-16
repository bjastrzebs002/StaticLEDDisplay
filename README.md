# StaticLEDDisplay
## Cel projektu
Celem projektu jest stworzenie wyświtlacza wyświtlającego dane informacje poprawnie (np. pionowo) w zależności od przychylenia/obrócenia wyświetlacza. Inną możliwością jest na tej samej podstawie stworzyć animację piłki jeżdżącej po ekranie w zależności od pochylenia go w którąś stronę.


## Analiza problemu
Żyroskop daje nam wartości w czasie rzeczywistym związane z przechyleniem go w 3 strony (X, Y, Z). Połączanie tych informacji i użycie jakiegoś algorytmu na przetworzenie tych informacji i w zależności od wyświetlanego obrazu na wyświetlaczu zmienić go względem pochylenia. Do tego zabiegu użyjemy techniki pooling, aby wiadomości z żyroskopu odbierać w czasie rzeczywistym i na bieżąca mieć możliwość zmiany wyświetlanego obrazu. 

## Schemat blokowy układu
Schemat blokowy działania projektu znajduje się w pliku block_schema.png


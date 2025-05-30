# Game of Life - Proiect C

Acest proiect implementează diverse versiuni ale simulării "Game of Life" folosind limbajul C, incluzând o versiune cu arbore binar de evoluție, stivă de generații și un checker Python pentru validarea outputului.

## Structura proiectului

- `task3.c` - Versiune avansată cu arbore binar de diferențe între generații 
- `task3.h` - Declarații, structuri și funcții auxiliare pentru task3
- `gameoflife.c` - Simulare clasică Game of Life cu afișarea fiecărei generații (functioneaza cel mai bine)
- `gol.c` - Task 2 - păstrează diferențele în stivă (fără undo) (file de baza care foloseste fisiere nu directoare)
- `bonus1.c` - Task 2 + Undo (reface matricea inițială) (bonus task 2, operatiile sunt facute dar nu sunt stocate intr-un output)
- `checker.py` - Script Python pentru compararea output-urilor generate (am folosit ai pentru ca nu imi mergea celalt checker)

## Compilare

```bash
gcc task3.c -o task3
gcc gameoflife.c -o gameoflife
gcc gol.c -o gol
gcc bonus1.c -o bonus1

#Utilizare
./task3 <folder_input> <folder_output>
./gameoflife <folder_input> <folder_output>
python3 checker.py <folder_output> <folder_expected>


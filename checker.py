import os
import sys

def citeste_linii_din_fisier(nume_fisier):
    # citeste toate liniile dintr-un fisier, eliminand liniile goale si spatiile
    try:
        with open(nume_fisier, 'r') as f:
            linii = [linie.strip() for linie in f.readlines() if linie.strip() != ""]
        return linii
    except FileNotFoundError:
        print(f"Eroare: Fișierul '{nume_fisier}' nu a fost găsit.")
        return []
    except IOError:
        print(f"Eroare: Nu s-a putut citi fișierul '{nume_fisier}'.")
        return []

def compara_doua_fisiere(fisier1, fisier2):
    # compara continutul a doua fisiere si afiseaza diferentele daca exista
    linii_fisier1 = citeste_linii_din_fisier(fisier1)
    linii_fisier2 = citeste_linii_din_fisier(fisier2)

    nume_fisier = os.path.basename(fisier1)
    if linii_fisier1 == linii_fisier2:
        print(f"{nume_fisier}: output corect")
    else:
        print(f"{nume_fisier}: output gresit")
        # afisam diferentele linie cu linie
        for index, (linie1, linie2) in enumerate(zip(linii_fisier1, linii_fisier2), start=1):
            if linie1 != linie2:
                print(f"  linia {index}: output = '{linie1}' | asteptat = '{linie2}'")
        # daca numarul de linii difera
        if len(linii_fisier1) != len(linii_fisier2):
            print(f"  numar diferit de linii: output are {len(linii_fisier1)}, asteptat are {len(linii_fisier2)}")

def verifica_toate_fisierele(folder_output, folder_expected):
    # compara toate fisierele .out din doua foldere si raporteaza rezultatele
    fisiere_output = sorted(f for f in os.listdir(folder_output) if f.endswith('.out'))
    fisiere_expected = sorted(f for f in os.listdir(folder_expected) if f.endswith('.out'))

    fisiere_comune = set(fisiere_output).intersection(fisiere_expected)

    if not fisiere_comune:
        print("nu s-au gasit fisiere .out comune in cele doua foldere")
        return

    for fisier in sorted(fisiere_comune):
        cale_output = os.path.join(folder_output, fisier)
        cale_expected = os.path.join(folder_expected, fisier)
        compara_doua_fisiere(cale_output, cale_expected)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("folosire: python checker.py <folder output> <folder expected>")
        sys.exit(1)

    folder_output = sys.argv[1]
    folder_expected = sys.argv[2]

    verifica_toate_fisierele(folder_output, folder_expected)

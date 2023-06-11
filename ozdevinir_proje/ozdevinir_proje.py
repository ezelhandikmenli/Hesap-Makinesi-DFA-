class turingMachine:
    # Girilen bir sayının karesini hesaplayan Turing makinesi tasarımı

    def __init__(self):
        self.durum = "Basla"
        self.bas_kisim = 0
        self.bant = []

    def run(self, bant):
        # Girilen değere göre bant üzerinde Turing Makinesi çalışır
        self.bant = bant
        sembol = self.bant[self.bas_kisim]
        
        while True:
            sembol = self.bant[self.bas_kisim]

            if self.durum == "Basla":
                if sembol == "Bos":
                    self.durum = "Kabul"
                else:
                    self.durum = "Çarpma_islemi"
                    self.bant[self.bas_kisim] = "1"
                    self.bas_kisim += 1

            elif self.durum == "Çarpma_islemi":
                if sembol == "Bos":
                    self.durum = "Kabul"
                else:
                    self.bant.append("1")
                    self.bas_kisim += 1

            elif self.durum == "Kabul":
                return
            

if __name__ == "__main__":
    n = int(input("Sayı giriniz: "))
    bant = ["Bos"] * n
    machine = turingMachine()
    machine.run(bant)
    print("{}'nin karesi {}'dir".format(n, len(machine.bant)**2))

# Remote Weight scale with GSM
Az `app` mappában találhatóak a program forrásfájljai. Ezek futtatásához STM32CubeIde 1.7.0 fejlesztői környezet szükséges.

A program futtatásához szükséges lépések:
- (Opcionálisan) Sim kártya PIN védettségének megszűntetése (például egy telefonnal).
- Sim kártya elhelyezése a GSM modul aljában.
- Az `\app\Core\Src\SIM900\SIM900.config.h` fájlban megadni a simkártyához tartozó PIN kódot illetve a cél telefonszámot (ahová az SMS küldeni fogja az alkalmazás).
- Az `` main függvényében módosítani lehet szükséges a mérleg kalibrációját melyet a `HX711_Calibrate(16405360, 1515 / (double)34575);` metódus paramétereinek módosításával lehet megtenni.
  - Az első paraméter a 0 grammnál mért érték legyen (kijelzőn látható).
  - A második paramétert a 0 gramhoz kalibrált állapottal lehet megkapni (a második paraméter ilyenkor legyen 1). Ezt követően egy imsert súlyt kell helyezni a mérlegre (mely legyen `N` gramm), ekkor a kijelzőn mért értékből megkapható a második paraméter: `N / mérés`.
- Ezt követően töltsük fel a kódot a lapra.
- Helyezzük áram alá a rendszert az USB porton keresztül.
- Kapcsoljuk be a GSM modult az odalán található nyomógombbal (1-2 secig nyomva tartva).
    - Ettől a GSM modulon két LED fog világítani, miután az egyik kialszik és ~3 másodpercenként villog a GSM modul rendebn elindult, ellenekző esetben próbálja újra bekapcsolni.
- Várjunk ~1-2 percet amíg meg nem érkezik az első SMS.

## Megjegyzések
A GSM modul érdemes lenne külső áramforrásról táplálni, ugyanis annak nagy energia igénye van és bekapcsoláskor sok esetben nem tud emiatt bekapcsolni.

A GSM modul indulására maximum 1 percet vár a program, ezután kilép. Tehát ezen időn belül kell sikeresen bekapcsolnunk a GSM modult.

A csatlakoztatott kijelzú jobb felső sarkában lévő LED világító állapotban jelzi, hogy még nem sikerült kapcsolódni a GSM modulhoz, ki kialszik sikeres volt a kapcsolódás.

Az SMS-eket az indulástól számítva 24 óránként küldi a program (ha tudja).
Közvetlenül az elindulás után küldi az első SMS-t (~1-2 percen belül), ezzel jelezve, hogy működőképes a rendszer.
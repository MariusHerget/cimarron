# Cimarron
## Erstes Meeting 4.9.18
Notizen:
- Funktionsschritte einzelnd aufführen
- Zwischenschritte speichern
- Preprocessing Schritte! z.B Histogramme
- Zweiter Schritt: Konzepte
- Einzelner Fall - Workproof usecase aufbräuseln --> Dummyimplementierung (Leichter versatz im Video) ist der Anfang
- Funktionsblock --> Konzept mit Ziel (funktionale Ebene)
- von SD auf Framework criteria
- Aufbröseln Sd
- Modelierung im Notfall möglich in der Theorie

Banaler Usecase:
- Beispielvideo

Nächste Schritte:
0. Paper in Slack schicken
1. Systemdiagram ausbauen
2. Minimalusecase
3. Konzeptenwtickeln

# Zweites Meeting 07.09.2018
- Domäne ist wichtig des Blocks (Eingang / Ausgang)
- Pre:
  - Video -> Frames Zeitdomain geht verloren
  - Überm File Name und unter dem Pfeil darstellungsform
Analysis
  - Frame t und Frame t+1
- Einzelne Frames pro block

Binarydata not good name
Every step tested, whole prudct not.

- Local Motion estimation genau definieren
- Usecase und daraus infos in SD
- Studiprohjekt Sachen rausnehmen

# Drittes Meeting 10.09.2018
Fragen:
  - V++ foreach_videoframe(videocstr) bereits die framesdata?
    - Color handling on the go
  - IO und Logik trennen nächster schritt, Modellierung SD komplette verarbeotungskette ()
  - Konzept enwitklcing (was muss was können und was brauch ich)
  - export über io << >> iomodifer iomanlib
  - png/bild format als modifer io

- vektor(x,y) und winkel getrennt
- Kategorie 1: Wie ensteht eint delta zwsichen zwei frame
- Kategorie 2: Instabilität, Rotation & Translation

PROZESSMODEL
System und beschränkungen klar rausschreiben

Verfahrne -> Konzepte (Modellierung)


# 9.10.2018
Konzeptformulieren (siehe CPP reference)
- Streamdarstellung:
  - Übergabe von streams, bzw einzelnen Frames -> Pipelining
  - Anpassung im Nachhinhein
  - MultiThreading
- Stream:
  - globale Überladung
  - Link vom Fuchs kommt TODO

- Smooth:
  - Ableitung Ort über Zeit (2D.)
  - 3/5 Punkt Ableitung
  - Theorie in die Presi

- Prozesskette
- Konzepte
- Smoothing evtl
- Code Cleanup

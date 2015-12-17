# Csirguru Bomber
First of the three university projects for computer graphics. OpenGL chicken bomber.

//TODO: translate project description

Magyar feladatleírás:
Készítsen CSIRGURU BOMBÁZÓ játékot. A csirguru a csirke és a kenguru nászából született jószág, csirke teste, csőre, szemei, taraja van, valamint egy lába, amivel kenguruszerűen ugrál. A lába a csípőízületben nem fordul el, a térízületben hátra fordul, a bokában előre (amit a normál csirke esetében hajlamosak vagyunk előre hajló térdnek hinni). Az egy szem lábujj ugyancsak felfelé hajlítható, az ízületek tengelyei egymással párhuzamosak.

A csirguruk testét néhány zárt Bézier görbével definiált keresztmetszetek Catmull-Rom spline-t alkalmazó transzfinit interpolációjával kell létrehozni. A szem, csör, taraj, lábrészek ismert egyenletű paraméteres felületekből (pl. gömb, kúp, henger, paraboloid stb.) összerakhatók.

A csirguruk egy sík, textúrázott és megvilágított terepen a bomba aktuális helye alatt, a program indításától számított másodpercenként születnek meg, majd elkezdenek ugrándozni. A lendületvételi fázisban a csirguru feneke a földhöz közelít, majd a térdízület EGYENES VONALÚ GYORSULÓ MOZGÁST (inverz kinematika) végez, mialatt a lábujj a talajon marad, a térd pedig vízszintesen tartja a csirgurut. A bokaízület kiegyenesedése pillantában a csirguru elszakad a földtől és lendületvétel végén elért kezdősebességű ferde hajítással leírható pályán ugrik, a levegőben előreteszi a lábát, a földetérkezésnél pedig a lendületvételt visszafelé játsza le. Az ugrálást ismételgetheti, az egyes ugrálások között a haladási irányát ízlésesen változtatva.

A felhasználó egy bombát irányíthat a mező fölött egy síkban az A, W, D, Y billentyűkkel, majd SPACE-re a bomba leesik. Ha a bomba egy csirguru közelében esik le, a csirguru testrészei független ferde hajítást követnek, mialatt midegyik testrész egy-egy forgástengely körül forog.

A színteret egy irányfényforrás (nap) és ambiens fényforrás világítja meg, a csirguruk és a bombák a talajra árnyékot (síkra vetített árnyék algoritmus) vetnek. 

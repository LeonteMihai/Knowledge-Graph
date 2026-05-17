[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/3SRI_-8J)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=23846869)
# Proiect PA 2025

Repository-ul conține scheletul proiectului și testele publice.

## Structură

`include/`
Headerele pentru modulele cerute.

`src/`
Fișierele pe care trebuie să le completați.

`tests/public/`
Teste publice pentru fiecare pas. Aici găsiți fișierele de intrare și ieșirea așteptată.

`build/`
Conține binarele pentru rularea testelor publice:
- `build/public_pas1`
- `build/public_pas2`
- `build/public_pas3`
- `build/public_pas4`

## Rulare teste publice

Pentru a rula toate testele publice:

```bash
make public-test
```

Pentru a rula testele publice doar pentru un pas:

```bash
./build/public_pas1 tests/public/pas1/entitati.csv tests/public/pas1/relatii.csv tests/public/pas1/interogari.txt
./build/public_pas2 tests/public/pas2/entitati.csv tests/public/pas2/relatii.csv tests/public/pas2/interogari.txt
./build/public_pas3 tests/public/pas3/entitati.csv tests/public/pas3/relatii.csv tests/public/pas3/interogari.txt
./build/public_pas4 tests/public/pas4/entitati.csv tests/public/pas4/relatii.csv tests/public/pas4/interogari.txt
```

Fiecare test public are și un fișier `expected.txt` cu rezultatul așteptat.

## Teste publice și teste private

Există două tipuri de teste:
- teste publice, incluse în acest repository;
- teste private, folosite la evaluare.

Faptul că testele publice trec nu garantează punctaj maxim.


## Descriere
 
Implementarea unui sistem de tip Knowledge Graph în limbajul C. Entitățile din lumea reală (persoane, companii, locații, evenimente) și relațiile dintre ele sunt organizate sub forma unui graf orientat ponderat. Sistemul suportă ingestia datelor din fișiere CSV, indexarea pentru căutare eficientă și răspunsul la interogări, inclusiv căutarea drumului de cost minim.
 
## Compilare și rulare
 
```bash
mkdir -p build
gcc -Wall -Wextra -g src/main.c src/bst.c src/entity.c src/graph.c \
    src/query.c src/queue.c src/heap.c -o build/knowledge_graph
 
./build/knowledge_graph <entitati.csv> <relatii.csv> <interogari.txt>
./build/knowledge_graph <entitati.csv> <relatii.csv> <interogari.txt> --debug
```
 
Modul `--debug` afișează la `stderr` lista de adiacență, BST-ul în inordine și coada de interogări, fără a afecta ieșirea standard.
 
## Structuri de date utilizate
 
### Listă simplu înlănțuită (graf cu liste de adiacență)
 
Graful este reprezentat printr-un tablou dinamic de `GraphNode`-uri, fiecare conținând o entitate și o listă înlănțuită de muchii de ieșire (`EdgeNode`). Alegerea listelor de adiacență față de matricea de adiacență este justificată de natura sparse a grafului — numărul de muchii este mult mai mic decât `n²`. Tabloul de noduri este indexat prin `id`, permițând acces O(1) la orice nod.
 
### Coadă FIFO (interogări)
 
Interogările sunt stocate într-o coadă implementată cu listă simplu înlănțuită, cu pointeri separați la `front` și `rear`. Această structură garantează enqueue și dequeue în O(1) și păstrează ordinea de procesare — primul intrat, primul ieșit — semantica naturală a unui query engine.
 
### Arbore binar de căutare — BST (indexare după nume)
 
BST-ul indexează entitățile după nume (comparație lexicografică cu `strcmp`). Nodul BST nu duplică datele entității — stochează doar un pointer la `GraphNode`-ul din graf. Construcția BST-ului direct din lista de entități (în ordinea citirii, garantat nesortată) asigură înălțimea medie O(log n), eliminând necesitatea unei variante auto-balansate (AVL/Red-Black).
 
### Min-Heap (Dijkstra)
 
Selecția nodului cu distanța minimă la fiecare pas al algoritmului Dijkstra este implementată cu un min-heap explicit, stocat ca tablou dinamic. Operațiile `sift_up` și `sift_down` mențin proprietatea de heap în O(log n). Alternativa — căutarea liniară — ar duce la complexitatea O(n²) pentru Dijkstra, inacceptabilă pe grafuri mari.
 
## Complexitatea operațiilor
 
| Operație | Complexitate | Justificare |
|---|---|---|
| EXISTS | O(log n) | Căutare în BST |
| EDGE | O(log n + grad(X)) | Căutare BST + parcurgere listă adiacență |
| NEIGHBORS | O(log n + grad(X)) | Căutare BST + parcurgere listă adiacență |
| PATH (BFS) | O(n + m) | Fiecare nod și muchie e vizitat cel mult o dată |
| DIJKSTRA | O((n + m) log n) | n extrageri din heap × O(log n) + m relaxări × O(log n) |
 
unde `n` = numărul de noduri, `m` = numărul de muchii.
 
## Algoritmii de traversare
 
### BFS — PATH
 
BFS explorează graful nivel cu nivel folosind o coadă de întregi (id-uri de noduri). Un vector `visited[]` previne revizitatrea nodurilor și evitarea ciclurilor. Vectorul `parents[]` rețin pentru fiecare nod descoperit nodul din care a fost atins, permițând reconstrucția drumului de la destinație înapoi la sursă. BFS garantează drumul cu numărul minim de muchii, nu neapărat costul minim.
 
### Dijkstra — DIJKSTRA
 
Dijkstra menține un vector `dist[]` inițializat cu `FLT_MAX` (infinit) și relaxează distanțele la fiecare pas. La fiecare iterație, nodul cu distanța curentă minimă este extras din min-heap. Dacă un nod e deja marcat `visited[]`, e ignorat — un nod poate apărea de mai multe ori în heap (câte o intrare per relaxare), dar e procesat o singură dată. Vectorul `parent[]` permite reconstrucția drumului după terminarea algoritmului.
 
Alegerea min-heap-ului față de căutarea liniară reduce complexitatea de la O(n²) la O((n + m) log n).
 

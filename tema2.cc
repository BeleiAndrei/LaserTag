// Copyright 2017 <Belei Andrei Iulian 312CA>
#include <iostream>
#include <list>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>


// Hashtable-ul va contine numele fiecarui jucator si indexul sau
template<typename Tkey, typename Tvalue>
struct elem_info {
    Tkey key;
    Tvalue value;
};

template<typename Tkey, typename Tvalue>
class Hashtable {
 private:
    std::list<struct elem_info<Tkey, Tvalue> > *H;
    int HMAX;
    unsigned int (*hash)(Tkey);

 public:
    Hashtable(int hmax, unsigned int (*h)(Tkey)){
        H = new std::list<struct elem_info<Tkey, Tvalue> > [hmax];
        HMAX = hmax;
        hash = h;
    }

    ~Hashtable(){
        delete [] H;
    }

    void put(Tkey key, Tvalue value){
        typename::std::list<struct elem_info<Tkey, Tvalue> >::iterator aux;
        aux = H[hash(key)].begin();
        for (; aux != H[hash(key)].end(); ++aux)
            if (aux->key == key){
                aux->value = value;
                return;
            }

        struct elem_info<Tkey, Tvalue> a;
        a.key = key;
        a.value = value;
        H[hash(key)].push_back(a);
    }

    void remove(Tkey key){
        typename::std::list<struct elem_info<Tkey, Tvalue> >::iterator aux;

        for (aux = H[hash(key)].begin(); aux != H[hash(key)].end(); ++aux)
            if (aux->key == key){
                H[hash(key)].erase(aux);
                    break;
                }
    }

    Tvalue get(Tkey key){
        typename::std::list<struct elem_info<Tkey, Tvalue> >::iterator aux;

        for (aux = H[hash(key)].begin(); aux != H[hash(key)].end(); ++aux)
            if (strcmp(aux->key, key) == 0){
                return aux->value;
            }
        return 0;
    }


    bool has_key(Tkey key){
        typename::std::list<struct elem_info<Tkey, Tvalue> >::iterator aux;

        for (aux = H[hash(key)].begin(); aux != H[hash(key)].end(); ++aux)
            if (aux->key == key){
                return true;
        }
        return false;
    }

    void print(){
        typename::std::list<struct elem_info<Tkey, Tvalue> >::iterator aux;
        int i;
        for (i = 0; i < HMAX; i++){
            std::cout << "H[" << i << "]->";
        for (aux=H[i].begin(); aux != H[i].end(); ++aux){
            std::cout << aux->key << "(" << aux->value << ")" << "    ";
        }
        std::cout << std::endl;
        }
    }
};

// Folosind aceasta clasa vom stoca date cu privire
// la impuscaturi si distanta intre senzori

class Graph {
 private:
    int ** matrix;
    int n;

 public:
    explicit Graph(int size){
        n = size;

        matrix = new int *[size];
        for (int i = 0; i < size; i++){
            matrix[i] = new int[size];
        }

        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++)
                matrix[i][j] = 0;
        }
    }

    ~Graph(){
        for (int i = 0; i < n; i++){
            delete [] matrix[i];
        }
        delete [] matrix;
    }

    // marcheaza o impuscatura
    void add_edge(int src, int dst){
        if (src == dst){
           return;
        }
        matrix[src][dst]++;
    }
    // adauga un nou senzor
    void add_sensor(int number, int *v){
        for (int i = 0; i < n; i++){
            if (number == i)
                matrix[number][i] = 0;
            else
            {
                if (number > i)
                    matrix[number][i] = v[i];
                else
                    matrix[number][i] = v[i - 1];
            }
        }
    }
    // returneaza numarul de impuscaturi sau distanta intre doi senzori
    int distance(int src, int dst){
        return matrix[src][dst];
    }
    // calculeaza scorul unui jucator
    int shooter_score(int key, int team, int teamsize){
        int i, score = 0;

        if (team == 1){
            for (i = 0; i < teamsize; i++)
                score = score + (-5) * matrix[key][i];
            for (i = teamsize; i< n; i++)
                score = score + 2 * matrix[key][i];
        }

        if (team == 2){
            for (i = 0; i < teamsize; i++)
                score = score + 2 * matrix[key][i];
            for (i = teamsize; i< n; i++)
                score = score + (-5) * matrix[key][i];
        }

        return score;
    }


    bool has_edge(int src, int dst){
        if (matrix[src][dst])
            return true;
        return false;
    }

    void print(){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++)
                std::cout  <<  matrix[i][j]  <<  " ";
            std::cout  <<  std::endl;
        }
    }
};

// Sorteaza in functie de punctaj clasamentul Top Shooter/Top Explorer
void quickSort(int **arr, int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[0][(left + right) / 2];

      while (i <= j) {
            while (arr[0][i] < pivot)
                  i++;
            while (arr[0][j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[0][i];
                  arr[0][i] = arr[0][j];
                  arr[0][j] = tmp;
                  tmp = arr[1][i];
                  arr[1][i] = arr[1][j];
                  arr[1][j] = tmp;
                  i++;
                  j--;
            }
      }

      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}

unsigned int hash(char* a){
    return (toupper(a[0])-'A');
}

// Sorteaza alfabetic clasamentul Top Shooter/Top Explorer
void alphabetical(int **arr, char **index, int size){
    int i, j, ok;
    int tmp;
    for (i = 0; i < size - 1; i++){
        ok = 0;
        for (j = i; j < size && arr[0][j] == arr[0][j + 1]; j ++)
            if (strcmp(index[arr[1][j]], index[arr[1][j + 1]]) < 0){
                  tmp = arr[1][j];
                  arr[1][j] = arr[1][j + 1];
                  arr[1][j + 1] = tmp;
                  ok = 1;
            }

        if (ok)
            i--;
    }
}


// Sorteaza alfabetic clasamentul Top Fire Exchange
void alphabeticalFE(int **arr, char **index, int inc, int size){
    int i, j, ok;
    int tmp;
    if (inc < 0)
        inc = 0;

    for (i = inc; i < size - 1; i++){
        ok = 0;
        for (j = i; j < size && arr[0][j] == arr[0][j + 1]; j++)
            if (strcmp(index[arr[1][j]], index[arr[1][j + 1]]) < 0){
                tmp = arr[1][j];
                arr[1][j] = arr[1][j + 1];
                arr[1][j + 1] = tmp;
                tmp = arr[2][j];
                arr[2][j] = arr[2][j + 1];
                arr[2][j + 1] = tmp;
                ok = 1;
            }
            else
            {
            if (strcmp(index[arr[2][j]], index[arr[2][j + 1]]) < 0){
                if (strcmp(index[arr[1][j]], index[arr[1][j + 1]]) == 0)
                {
                    tmp = arr[2][j];
                    arr[2][j] = arr[2][j + 1];
                    arr[2][j + 1] = tmp;
                    ok = 1;
                }
            }
            }

        if (ok)
            i--;
    }

    if (size > 0 && arr[0][size] == arr[0][size - 1]){
        if (strcmp(index[arr[1][size]], index[arr[1][size - 1]]) > 0)
        {
            tmp = arr[1][size];
            arr[1][size] = arr[1][size - 1];
            arr[1][size - 1] = tmp;
            tmp = arr[2][size];
            arr[2][size] = arr[2][size - 1];
            arr[2][size - 1] = tmp;
            ok = 1;
        }
        else
        {
        if (strcmp(index[arr[2][size]], index[arr[2][size - 1]]) > 0){
            if (strcmp(index[arr[1][size]], index[arr[1][size - 1]]) == 0)
            {
                tmp = arr[2][size];
                arr[2][size] = arr[2][size - 1];
                arr[2][size - 1] = tmp;
                ok = 1;
            }
        }
        }
    }
}


int main(){
    char *input, *pointer, *target, **index, *saveptr;
    int i, j, k, ns, n, m, sensor, t, *poz, value;
    int shootout, score[2] = {0}, top;
    double WC[2] = {0};

    input = new char[50];
    target = new char[50];

    // Citirea si memorearea datelor despre senzori
    std::cin >> ns;
    Graph sensors(ns);
    if (ns > 0){
        int *v;
        v = new int[ns-1];

        for (i = 0; i < ns; i++){
            for (j = 0; j < ns - 1; j++)
                std::cin  >>  v[j];
            sensors.add_sensor(i, v);
        }
        delete[] v;
    }

    // Citirea numarului de jucatori si
    // alocarea de memorie in functie de acestia
    std::cin >> n >> m;
    Graph Shootings(n + m);
    int **top_shooter;
    int **top_explorer;
    int **top_fire;
    int vis[n+m][ns] = {0};
    int alive[n+m] = {0};

    top_shooter = new int *[2];
    top_shooter[0] = new int[n + m + 1];
    top_shooter[1] = new int[n + m + 1];

    top_explorer = new int *[2];
    top_explorer[0] = new int[n + m + 1];
    top_explorer[1] = new int[n + m + 1];

    top_fire = new int *[3];
    top_fire[0] = new int[(n + m) * (n + m) / 2];
    top_fire[1] = new int[(n + m) * (n + m) / 2];
    top_fire[2] = new int[(n + m) * (n + m) / 2];

    memset(top_explorer[0], 0, 4*(n + m + 1));

    Hashtable <char*, int> Players(26, hash);

    index = new char*[n + m + 1];
    poz = new int[n + m + 2];
    for (i = 0; i < n + m + 1; i++)
        index[i] = new char[50];

    // Memorarea jucatorilor
    for (i = 0; i< n + m; i++){
        char *player = new char[50];
        scanf("%s", player) == 1;
        strncpy(index[i], player, strlen(player)+1);
        Players.put(index[i], i);
        delete [] player;
    }

    scanf("%s", input) == 1;
    while (strcmp(input, "END_CHAMPIONSHIP")){
        if (input[0] < 'A'){
            // Caz in care un jucator activeaza un senzor
            pointer = strtok_r(input, ":", &saveptr);
            sensor = atoi(pointer);
            scanf("%s", input) == 1;
            value = Players.get(input);

            // Modificarea scorului de explorator al jucatorului
            if (poz[value] == -1)
                top_explorer[0][value] += 3;
            else
            {
                top_explorer[0][value] += abs(vis[value][sensor] - 1) * 3;
                top_explorer[0][value] += sensors.distance(poz[value], sensor);
            }
            poz[value] = sensor;
            vis[value][sensor] = 1;
        }
            else
            {
            if (strncmp(input, "JOC", 3) == 0){
                // Joc nou
                // Resetarea pozitiei, senzorilor vizitati si
                // vietilor unui jucator
                memset(poz, -1, 4 * (n + m + 1));
                if (alive[0] || alive[1]){
                    if (alive[0] == 0)
                        score[1]++;
                    else
                        score[0]++;
                }
                memset(alive, 0, 4 * (n + m + 3));
                alive[0] = n;
                alive[1] = m;
                for (i = 0; i < n + m; i++){
                    memset(vis[i], 0, 4 * (ns + 1));
                }
            }
            else
            {
                // Caz in care un jucator este impuscat
                scanf("%s", target) == 1;
                scanf("%s", target) == 1;
                t = Players.get(target);
                Shootings.add_edge(Players.get(input), t);
                alive[t + 2]--;
                if (alive[t + 2] == -2){
                    if (t >= n)
                        alive[1]--;
                    else
                        alive[0]--;
                }
            }
        }
        scanf("%s", input) == 1;
    }
    // t marcheaza echipa
    t = 1;
    // Calcularea scorulu de tragator al fiecarui jucator
    for (i = 0; i < n + m; i++){
        if (i >= n)
            t = 2;
        top_shooter[0][i] = Shootings.shooter_score(i, t, n);
        top_shooter[1][i] = i;
        top_explorer[1][i] = i;
    }

    // Calculare Winning Chance
    if (alive[0] != 0 && alive[1] != 0){
        int maxS = 0, maxE = 0;
        for (i = 0; i < n + m; i++){
            if (maxS < top_shooter[0][i])
                maxS = top_shooter[0][i];
            if (maxE < top_explorer[0][i])
                maxE = top_explorer[0][i];
        }
        for (i = 2; i < n + 2; i++){
            if (alive[i] != -2){
                if (maxS != 0)
                    WC[0] += top_shooter[0][i - 2]/(double)maxS;
                if (maxE != 0)
                    WC[0] += top_explorer[0][i - 2]/(double)maxE;
            }
            }
        for (i = n + 2; i < n + m + 2; i++){
            if (alive[i] != -2){
                if (maxS != 0)
                    WC[1] += top_shooter[0][i - 2]/(double)maxS;
                if (maxE != 0)
                    WC[1] += top_explorer[0][i - 2]/(double)maxE;
            }
            }
        WC[0] = round(WC[0] * 1000) / 1000;
        WC[1] = round(WC[1] * 1000) / 1000;
        if (WC[0] > WC[1])
            score[0]++;
        else
            score[1]++;
    }
        else
        {
            if (alive[0] == 0)
                score[1]++;
            else
                score[0]++;
        }
    // Sortare si afisare al clasamentului Top Shooters
    quickSort(top_shooter, 0, n + m - 1);
    alphabetical(top_shooter, index, n + m - 1);
    std::cout << "I. Top shooters" << std::endl;
    top = 5;
    for (i = n + m - 1; i >= 0 && i >= n + m - top; i--){
        std::cout << n+m-i << ". ";
        std::cout << index[top_shooter[1][i]] << " ";
        std::cout << top_shooter[0][i] << "p" << std::endl;
        if (i > 0 && top_shooter[0][i] == top_shooter[0][i - 1])
            if (i == (n + m -top))
            top++;
    }

    // Sortare si afisare al clasamentului Top Explorers
    quickSort(top_explorer, 0, n + m - 1);
    alphabetical(top_explorer, index, n + m - 1);
    std::cout << std::endl << "II. Top explorers" << std::endl;
    top = 5;
    for (i = n + m - 1; i >= 0 && i >= n + m - top; i--){
        std::cout << n + m - i << ". ";
        std::cout << index[top_explorer[1][i]] << " ";
        std::cout << top_explorer[0][i] << "p" << std::endl;
        if (i > 0 && top_explorer[0][i] == top_explorer[0][i - 1])
            if (i == (n + m -top))
                top++;
    }

    std::cout << std::endl << "III. Top fire exchange" << std::endl;

    // Calcularea si stocare clasamentului de fire exchange
    // k reprezinta pozitia in vector
    k = 0;
    for (i = 0; i < n + m - 1; i++){
        for (j = i + 1; j < n + m; j++){
            shootout = Shootings.distance(i, j) + Shootings.distance(j, i);
            top_fire[0][k] = shootout;
            if (strcmp(index[i], index[j]) > 0){
                top_fire[1][k] = j;
                top_fire[2][k] = i;
            }
                else
                {
                    top_fire[1][k] = i;
                    top_fire[2][k] = j;
                }
            k++;
        }
    }

    // Sortare in functie de scor
    int tmp;
    for  ( int pass = 0; pass < k - 2; pass++ )
    {
        for  ( int j = 0; j < k -1; j++ )
        {
            if ( top_fire[0][j] > top_fire[0][j + 1] )
            {
                tmp = top_fire[0][j];
                top_fire[0][j] = top_fire[0][j + 1];
                top_fire[0][j + 1] = tmp;
                tmp = top_fire[1][j];
                top_fire[1][j] = top_fire[1][j + 1];
                top_fire[1][j + 1] = tmp;
                tmp = top_fire[2][j];
                top_fire[2][j] = top_fire[2][j + 1];
                top_fire[2][j + 1] = tmp;
            }
        }
    }

    // Calcularea dimensiunii topului
    top = 5;
    for (i = k - 1; i >= 0 && i>= k - top; i--){
        if (i > 0 && top_fire[0][i] == top_fire[0][i - 1] && i == (k - top))
            top++;
    }

    // Sortarea alfabetica a topului
    alphabeticalFE(top_fire, index, k - top - 1 , k - 1);

    // Afisarea clasamentului Top Fire exchange
    for (i = k - 1; i >= 0 && i > k - top - 1; i--){
        std::cout << k-i << ". ";
        std::cout << index[top_fire[1][i]] << " - ";
        std::cout << index[top_fire[2][i]] << " ";
        std::cout << top_fire[0][i] << std::endl;
    }

    // Afisarea scorului final
    std::cout << std::endl << "IV. Final score" << std::endl;
    std::cout << score[0] << " - " << score[1] << std::endl;

    delete [] poz;
    delete [] input;
    delete [] target;
    delete [] top_shooter[0];
    delete [] top_shooter[1];
    delete [] top_shooter;
    delete [] top_explorer[0];
    delete [] top_explorer[1];
    delete [] top_explorer;
    delete [] top_fire[0];
    delete [] top_fire[1];
    delete [] top_fire[2];
    delete [] top_fire;
    for (i = 0; i < n + m + 1; i++)
        delete [] index[i];
    delete [] index;

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

ifstream in("intrare.in");
ofstream out("iesire.out");

const int nmax = 100001;
const int inf = 1<<30;
class Graf
{
private:
    bool orientat;
    int nrNoduri;
    vector <int> listaAdiacenta[nmax];
    vector <pair <int, int>> listaAdiacentaCost[nmax];
public:
    Graf(int nrNoduri = 0, bool orientat = false);
    void adaugaMuchie(int, int);
    void adaugaMuchieCost(int, int, int);
    void citireMuchii(int);
    void citireMuchiiCost(int);
    vector<int> BFS(int);
    void DFS(int, bool*);
    int nrComponenteConexe();
    void DFSStiva(int nodcurent, bool *, stack<int> &);
    void sortareTopologica();
    void TarjanCTC(int&, int, int*, int*, bool*, stack <int> &, vector<vector<int>> &);
    void CTC();
    void TarjanBC(int&, int, int, int*, int*, stack <int>&, vector<vector<int>>&);
    void BC();
    void TarjanMC(int&, int, int, int*, int*, vector<pair<int,int>>&);
    void MC();
    vector <int> citireHakimi();
    bool Hakimi(vector <int>);
    vector <int> Dijkstra(int);
    pair<int, vector <pair <int, int>>> Prim(int);
    vector <int> BellmanFord(int);
    void reuniune(int, int, vector<int>&, vector<int>&);
    int gasire(int, vector<int>&);
    void verifica(int, int, vector<int>&);
    void paduri();
};

Graf :: Graf(int nrNoduri, bool orientat) : nrNoduri(nrNoduri), orientat(orientat)
{
    this->nrNoduri = nrNoduri;
    this->orientat = orientat;
}

void Graf:: adaugaMuchie(int nod1, int nod2)
{
    listaAdiacenta[nod1].push_back(nod2);
}

void Graf::adaugaMuchieCost(int nod1, int nod2, int cost)
{
    listaAdiacentaCost[nod1].push_back(make_pair(nod2, cost));
}

void Graf::citireMuchii(int nrMuchii)
{
    int nod1, nod2;
    for(int i = 0; i < nrMuchii; i++)
    {
        in >> nod1 >> nod2;
        listaAdiacenta[nod1].push_back(nod2);
        if(!orientat)
        {
            listaAdiacenta[nod2].push_back(nod1);
        }
    }
}

void Graf::citireMuchiiCost(int nrMuchii)
{
    int nod1, nod2, cost;
    for(int i = 0; i < nrMuchii; i++)
    {
        in >> nod1 >> nod2 >> cost;
        adaugaMuchieCost(nod1, nod2, cost);
        if(!orientat)
        {
            adaugaMuchieCost(nod2, nod1, cost);
        }
    }
}

vector<int> Graf::BFS(int start)
{
    queue <int> coadaBFS;
    int vizitatBFS[nmax] = {false};
    vector <int> distantaBFS(nmax);
    fill(distantaBFS.begin(), distantaBFS.end(), 0);
    coadaBFS.push(start);
    vizitatBFS[start] = true;
    distantaBFS[start] = 1;
    while(!coadaBFS.empty())
    {
        int nodCurent = coadaBFS.front();
        coadaBFS.pop();
        for(auto vecin:listaAdiacenta[nodCurent])
        {
            if(!vizitatBFS[vecin])
            {
                vizitatBFS[vecin] = true;
                distantaBFS[vecin] = distantaBFS[nodCurent] + 1;
                coadaBFS.push(vecin);
            }
        }
    }
    return distantaBFS;
}

void Graf::DFS(int nodCurent, bool *vizitatDFS)
{
    vizitatDFS[nodCurent] = true;
    for(auto vecin:listaAdiacenta[nodCurent])
    {
        if(!vizitatDFS[vecin])
        {
            DFS(vecin, vizitatDFS);
        }
    }
}

int Graf::nrComponenteConexe()
{
    int contorComponente = 0;
    bool vizitat[nmax] = {false};
    for(int i = 1; i <= nrNoduri; i++)
    {
        if(!vizitat[i])
        {
            DFS(i, vizitat);
            contorComponente++;
        }
    }
    return contorComponente;
}

void Graf::DFSStiva(int nodCurent, bool *vizitatDFS, stack<int> &stivaTimp)
{
    vizitatDFS[nodCurent] = true;
    for(auto vecin:listaAdiacenta[nodCurent])
    {
        if (!vizitatDFS[vecin])
        {
            DFSStiva(vecin, vizitatDFS, stivaTimp);
        }
    }
    stivaTimp.push(nodCurent);
}

void Graf::sortareTopologica()
{
    bool vizitat[nmax] = {false};
    stack <int> stivaTimp;
    for(int i = 1; i <= nrNoduri; i++)
    {
        if(!vizitat[i])
        {
            DFSStiva(i, vizitat, stivaTimp);
        }
    }
    while(!stivaTimp.empty())
    {
        out << stivaTimp.top() << " ";
        stivaTimp.pop();
    }
}

void Graf::TarjanCTC(int &timpCurent, int nodCurent, int *timpDescoperire, int *minimLegat, bool *estePeStiva, stack <int> &stiva, vector<vector<int>> &componenteTConexe)
{
    stiva.push(nodCurent);
    estePeStiva[nodCurent] = true;
    timpDescoperire[nodCurent] = timpCurent;
    minimLegat[nodCurent] = timpCurent;
    timpCurent++;
    for(auto vecin:listaAdiacenta[nodCurent])
    {
        if(timpDescoperire[vecin] == 0)
        {
            TarjanCTC(timpCurent, vecin, timpDescoperire, minimLegat, estePeStiva, stiva,componenteTConexe);
            minimLegat[nodCurent] = min(minimLegat[nodCurent], minimLegat[vecin]);
        }
        else if(estePeStiva[vecin])
        {
            minimLegat[nodCurent] = min(minimLegat[nodCurent], minimLegat[vecin]);
        }
    }
    if(timpDescoperire[nodCurent] == minimLegat[nodCurent])
    {
        vector <int> componentaCurenta;
        while(stiva.top() != nodCurent)
        {
            componentaCurenta.push_back(stiva.top());
            estePeStiva[stiva.top()] = false;
            stiva.pop();
        }
        componentaCurenta.push_back(stiva.top());
        estePeStiva[stiva.top()] = false;
        stiva.pop();
        componenteTConexe.push_back(componentaCurenta);
    }
}

void Graf::CTC()
{
    int timpCurent = 1;
    int timpDescoperire[nmax] = {0}, minimLegat[nmax] = {0};
    bool estePeStiva[nmax] = {false};
    stack <int> stiva;
    vector <vector <int>> componenteTConexe;
    for(int i = 1; i <= nrNoduri; i++)
    {
        if(timpDescoperire[i] == 0)
        {
            TarjanCTC(timpCurent, i, timpDescoperire, minimLegat, estePeStiva, stiva, componenteTConexe);
        }
    }
    out << componenteTConexe.size() << "\n";
    for(auto x:componenteTConexe)
    {
        for(auto y:x)
        {
            out << y << " ";
        }
        out << "\n";
    }
}

void Graf::TarjanBC(int &timpCurent, int nodCurent, int parinte, int *timpDescoperire, int *minimLegat, stack <int> &stiva, vector<vector<int>> &componenteBConexe)
{
    stiva.push(nodCurent);
    timpDescoperire[nodCurent] = timpCurent;
    minimLegat[nodCurent] = timpCurent;
    timpCurent++;
    for(auto vecin:listaAdiacenta[nodCurent])
    {
        if(vecin != parinte)
        {
            if(timpDescoperire[vecin] == 0)
            {
                TarjanBC(timpCurent, vecin, nodCurent, timpDescoperire, minimLegat, stiva, componenteBConexe);
                minimLegat[nodCurent] = min(minimLegat[nodCurent], minimLegat[vecin]);
                if (timpDescoperire[nodCurent] <= minimLegat[vecin]) {
                    vector<int> componentaCurenta;
                    int aux;
                    componentaCurenta.push_back(nodCurent);
                    do {
                        aux = stiva.top();
                        componentaCurenta.push_back(aux);
                        stiva.pop();
                    } while (aux != vecin);
                    componenteBConexe.push_back(componentaCurenta);
                }
            }
            else
            {
                minimLegat[nodCurent] = min(minimLegat[nodCurent], timpDescoperire[vecin]);
            }
        }
    }
}

void Graf::BC()
{
    int timpCurent = 1;
    int timpDescoperire[nmax] = {0}, minimLegat[nmax] = {0};
    stack <int> stiva;
    vector <vector <int>> componenteBConexe;
    TarjanBC(timpCurent, 1, -1, timpDescoperire, minimLegat, stiva, componenteBConexe);
    out << componenteBConexe.size() << "\n";
    for(auto x:componenteBConexe)
    {
        for(auto y:x)
        {
            out << y << " ";
        }
        out << "\n";
    }
}

void Graf::TarjanMC(int &timpCurent, int nodCurent, int parinte, int *timpDescoperire, int *minimLegat, vector<pair<int, int>> &muchiiCritice)
{

    timpDescoperire[nodCurent] = timpCurent;
    minimLegat[nodCurent] = timpCurent;
    timpCurent++;
    for(auto vecin:listaAdiacenta[nodCurent])
    {
        if(vecin != parinte)
        {
            if(timpDescoperire[vecin] == 0)
            {
                TarjanMC(timpCurent, vecin, nodCurent, timpDescoperire, minimLegat, muchiiCritice);
            }
            minimLegat[nodCurent] = min(minimLegat[nodCurent], minimLegat[vecin]);
            if(timpDescoperire[nodCurent] < minimLegat[vecin])
            {
                muchiiCritice.push_back(make_pair(nodCurent,vecin));
            }
        }
    }
}

void Graf::MC()
{
    int timpCurent = 1;
    int timpDescoperire[nmax] = {0}, minimLegat[nmax] = {0};
    stack<int> stiva;
    vector<pair<int,int>> muchiiCritice;
    TarjanMC(timpCurent, 1, -1, timpDescoperire, minimLegat, muchiiCritice);
    cout << muchiiCritice.size() << "\n";
    for (auto x:muchiiCritice) {
        cout << x.first << " " << x.second;
        cout << "\n";
    }
}

vector <int> Graf::citireHakimi()
{
    int n, x;
    vector <int> v;
    cin >> n;
    for(int i = 0; i < n; i++)
    {
        cin >> x;
        v.push_back(x);
    }
    return v;
}

bool Graf::Hakimi(vector <int> grade)
{
    sort(grade.begin(), grade.end(),greater<>());
    while(!grade.empty())
    {
        int x = grade[0];
        grade.erase(grade.begin());
        if(x > grade.size())
        {
            return false;
        }
        for(int i = 0; i < x; i++)
        {
            if(--grade[i] < 0)
            {
                return false;
            }
        }
        sort(grade.begin(), grade.end(),greater<>());
    }
    return true;
}

vector <int> Graf::Dijkstra(int nodStart)
{
    vector <int> distanta(nmax);
    fill(distanta.begin(), distanta.end(), inf);
    priority_queue <pair <int, int>, vector <pair <int, int>>, greater<pair<int,int>>> coada;
    bool vizitat[nmax] = {0};
    distanta[nodStart] = 0;
    coada.push(make_pair(0,nodStart));
    while(!coada.empty())
    {
        int nodCurent = coada.top().second;
        coada.pop();
        if(!vizitat[nodCurent])
        {
            vizitat[nodCurent] = true;
            for(auto vecin:listaAdiacentaCost[nodCurent])
            {
                if(!vizitat[vecin.first])
                {
                    if(distanta[nodCurent] + vecin.second < distanta[vecin.first])
                    {
                        distanta[vecin.first] = distanta[nodCurent] + vecin.second;
                        coada.push(make_pair(distanta[vecin.first], vecin.first));
                    }
                }
            }
        }
    }
    return distanta;
}

pair<int, vector <pair <int, int>>> Graf::Prim(int nodStart)
{
    vector<pair<int, int>> rezultat;
    vector<int> cost(nmax);
    fill(cost.begin(), cost.end(), inf);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> coada;
    bool vizitat[nmax] = {0};
    int costTotal = 0;
    vector<int> parinte(nmax);
    cost[nodStart] = 0;
    coada.push(make_pair(0, nodStart));
    while (!coada.empty()) {
        int nodCurent = coada.top().second;
        coada.pop();
        if (!vizitat[nodCurent]) {
            vizitat[nodCurent] = true;
            costTotal += cost[nodCurent];
            if (parinte[nodCurent] > 0) {
                rezultat.push_back(make_pair(nodCurent, parinte[nodCurent]));
            }
            for (auto vecin:listaAdiacentaCost[nodCurent]) {
                if (!vizitat[vecin.first]) {
                    if (vecin.second < cost[vecin.first]) {
                        cost[vecin.first] = vecin.second;
                        parinte[vecin.first] = nodCurent;
                        coada.push(make_pair(cost[vecin.first], vecin.first));
                    }
                }
            }
        }
    }
    return make_pair(costTotal, rezultat);
}

vector <int> Graf::BellmanFord(int nodStart)
{
    vector <int> cost(nmax, inf);
    queue<int> coada;
    bool esteInCoada[nmax] = {false};
    vector <int> nrIteratii(nmax,0);
    cost[nodStart] = 0;
    coada.push(nodStart);
    esteInCoada[nodStart] = true;
    nrIteratii[nodStart]++;
    while(!coada.empty())
    {
        int nodCurent = coada.front();
        coada.pop();
        esteInCoada[nodCurent] = false;
        if(nrIteratii[nodCurent] == nrNoduri)
        {
            out << "Ciclu negativ!";
            cost.clear();
            return cost;
        }
        nrIteratii[nodCurent]++;
        for (auto vecin:listaAdiacentaCost[nodCurent])
        {
            if(cost[nodCurent] + vecin.second < cost[vecin.first])
            {
                cost[vecin.first] = cost[nodCurent] + vecin.second;
                if(!esteInCoada[vecin.first])
                {
                    coada.push(vecin.first);
                    esteInCoada[vecin.first] = true;
                }
            }
        }
    }

    return cost;
}

void Graf::reuniune(int nod1i, int nod2i, vector <int> &parinte, vector <int> &rang)
{
    int nod1 = gasire(nod1i,parinte);
    int nod2 = gasire(nod2i, parinte);
    if(rang[nod1] > rang[nod2])
    {
        parinte[nod1] = nod2;
    }
    else if(rang[nod2] <rang[nod1])
    {
        parinte[nod2] = nod1;
    }
    else
    {
        parinte[nod2] = nod1;
        rang[nod2]++;
    }
}

int Graf::gasire(int nod, vector <int> &parinte)
{
    if(parinte[nod] == -1)
    {
        return nod;
    }
    parinte[nod] = gasire(parinte[nod], parinte);
    return parinte[nod];
}

void Graf::verifica(int nod1, int nod2, vector<int>&parinte)
{
    if(gasire(nod1,parinte) == gasire(nod2, parinte))
    {
        out << "DA\n";
        return;
    }
    out << "NU\n";
}

void Graf::paduri()
{
    int n, m;
    in >> n >> m;
    int nod1, nod2, operatie;
    vector <int> parinte(nmax, -1);
    vector <int> rang(nmax, 0);
    for(int i = 0 ; i < m ; i++)
    {
        in >> operatie >> nod1 >> nod2;
        if(operatie == 1)
        {
            reuniune(nod1, nod2, parinte, rang);
        }
        else if(operatie == 2)
        {
            verifica(nod1, nod2, parinte);
        }
    }
}

int main() {
    int n, m;
    in >> n >> m;
    Graf g(n,true);
    //aici se apeleaza functiile pentru oricare din probleme
    return 0;
}

/*Sursele acceptate pe infoarena/leetcode:
 * DFS: https://www.infoarena.ro/job_detail/2793349?action=view-source
 * BFS: https://www.infoarena.ro/job_detail/2793366?action=view-source
 * Sortare topologica: https://www.infoarena.ro/job_detail/2793366?action=view-source
 * Componente tare conexe: https://www.infoarena.ro/job_detail/2796467?action=view-source
 * Componente biconexe: https://www.infoarena.ro/job_detail/2797668?action=view-source
 * Critical connections: https://leetcode.com/submissions/detail/584947814/
 * Dijkstra: https://www.infoarena.ro/job_detail/2802636?action=view-source
 * APM: https://www.infoarena.ro/job_detail/2802984?action=view-source
 * BellmanFord: https://www.infoarena.ro/job_detail/2803009?action=view-source
 * Disjoint: https://www.infoarena.ro/job_detail/2805986?action=view-source
 */

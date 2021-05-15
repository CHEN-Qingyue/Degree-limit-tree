#include<cstdio>
#include<iostream>
#include<map>
#include<list>
#include<vector>
#include<algorithm>
#include<cstring>


#define N 7     //Le nombre de sommets dans le graphe d'origine
#define INF 100


using namespace std;

int size = N * sizeof(int);

// classe de arete
class Edge {
public:
    int src, dest, poids; //source,destination et poids pour chaque arete

    // classer les arêtes par les poids de ordre décroissant 
    friend bool operator < (Edge e1, Edge e2) {
        return e1.poids < e2.poids;
    }
};

class Arbre {
public:
    int sommets[N];
    list <class Edge> edges;
    Arbre();
};

Arbre::Arbre() {
    for (int i = 0; i < N; i++) {
        sommets[i] = 0;
    }
}

// classe pour le graphique (arbre)
class Graph {
public:
    int V, E;    //nombre de sommets et d'aretes
    int sommets[N]; //liste de sommets
    vector <class Edge> edges;  // liste d'aretes
    int g[N][N];  // liste d'adjacent (valeur)
    int father[N]; // parent pour les sommets 
    int limite[N]; //limite de degre pour chaque sommet
    int mini;
    vector<class Graph> sousG; //les sous-ensembles de graphe
    vector<Edge> connexion;
    vector<class Arbre> Tmini; //les arbres couvrants
    Graph();
    Graph(int V, int E, int s[N]);
};

Graph::Graph() {

    //edges = new list<class Edge>[E];  // créer un liste d'arete de taille E
    this->mini = 0;
    // initialisation
    for (int i = 0; i < N; i++) {
        sommets[i] = 0;
        for (int j = 0; j < N; j++)
            g[i][j] = INF;
    }
    //edeges.sort(); //Trier les aretes du plus petit au plus grand
}

Graph::Graph(int V, int E, int s[N]) :V(V), E(E)
{

    memcpy(sommets, s, size);
    //edges = new list<class Edge>[E];  // créer un liste d'arete de taille E
    this->mini = 0;
    // initialisation
    /*
    for(int i=0;i<V;i++){
        this->father[i] = i;
        this->sommets[i]=1;
    }
    */
    sort(this->edges.begin(), this->edges.end()); //Trier les aretes du plus petit au plus grand
}

int creatEdges(Graph& G) {
    int nbE = 0;
    Edge* p;
    for (int i = 0; i < N; i++)
        for (int j = i + 1; j < N; j++) {
            if (G.g[i][j] != INF) {
                nbE += 1;
                p = new Edge;
                p->src = i;
                p->dest = j;
                p->poids = G.g[i][j];
                //cout<<i<<","<<j<<endl;
                G.edges.push_back(*p);
                delete p;
            }
        }
    return nbE;
}

// fonction pour trouver le parent pour chaque sommet
int findfather(Graph G, int x) {
    if (x != G.father[x]){
        G.father[x] = findfather(G, G.father[x]);
    }
    return G.father[x];
}

void regrouper(Graph& G) {
    int root, flag = 1;
    int nonmarque[N];
    //int nG = 0; //nombre de sous-graph
    Graph* p;

    //Initialisation father[N]
    for (int i = 0; i < N; i++) {
      G.father[i] = i;
      if (flag && G.sommets[i] == 1) {
        root = i;
        flag = 0;
      }
    }

    for (int i = 0; i < G.E; i++) {
        if (G.sommets[G.edges[i].src] == 0 || G.sommets[G.edges[i].dest] == 0 || G.edges[i].src == root || G.edges[i].dest == root) continue;
        int find_src = findfather(G, G.edges[i].src);
        int find_dest = findfather(G, G.edges[i].dest);
        if (find_src == find_dest) continue;
        //flag[G.edges[i].src][G.edges[i].dest] = flag[G.edges[i].dest][G.edges[i].src] = true;
        G.father[find_src] = find_dest;
    }

  for (int i = 0; i < N; i++) {
      nonmarque[i] = 1;
  }
  for (int i = 0; i < N; i++) {
      if ((G.sommets[i]) && (nonmarque[i]) && (i != root)) {
          //cout<<i<<endl;
          p = new Graph;
          p->sommets[i] = 1;
          nonmarque[i] = 0;
          //cout<<findfather(G,5)<<findfather(G,6)<<endl;
          for (int j = 0; j < N; j++) {
              //cout<<i<<j<<":"<<endl;
              if ((i != j) && findfather(G, i) == findfather(G, j)) {
                  //cout<<findfather(G,i)<<findfather(G,j)<<endl;
                  p->sommets[j] = 1;
                  nonmarque[j] = 0;
                  for (int k = 0; k < j; k++) {
                      if (k == root) continue;
                      p->g[j][k] = p->g[k][j] = G.g[j][k];

                  }
              }
          }
          p->E = creatEdges(*p);
          memcpy(p->limite, G.limite, N * sizeof(int));
          G.sousG.push_back(*p);
          delete p;
      }

    }


    vector<Graph>::iterator iter;
    Edge* q;

    for (int i = root + 1; i < N; i++) {
        if (G.g[root][i] != INF) {
            //cout<<i<<endl;
            for (iter = G.sousG.begin(); iter != G.sousG.end(); ++iter) {
                if (iter->sommets[i] != 0) {
                    q = new Edge;
                    q->src = root;
                    q->dest = i;
                    q->poids = G.g[root][i];
                    iter->connexion.push_back(*q);
                    delete q;
                    break;
                }
            }
        }
    }

}

// trouver les arbres couvrants minimum
void minimum(Graph G, Arbre T) {
    int mini = 0;
    int sommeT = 0;
    if (G.Tmini != NULL){
        for (i=(G.Tmini[0]->edgeArbre).begin();i!=(G.Tmini[0]->edgeArbre).end();i++)
        {
            mini += i->poids;
        }
        for (j=(T->edgeArbre).begin();i!=(T->edgeArbre).end();i++)
        {
            sommeT += i->poids;
        }
        if (sommeT < mini)
        {
            G.Tmini = NULL;
            G.Tmini[0] = T;   
        }
        else if (sommeT == mini)
        {
            G.Tmini += T;
        }
    } 
    G.Tmini[0] = T;
    
}


int main() {
    int s[7] = { 1,1,1,1,1,1,1 };
    int lim[N] = { 3,3,3,3,3,3,3 };
    Graph G1(7, 8, s);
    int a[N][N] = { {INF,1,INF,INF,2,1,INF},
            {1,INF,1,INF,5,INF,INF},
            {INF,1,INF,2,6,INF,INF},
            {INF,INF,2,INF,INF,INF,INF},
            {2,5,6,INF,INF,INF,INF},
            {1,INF,INF,INF,INF,INF,1},
            {INF,INF,INF,INF,INF,1,INF} };
    memcpy(G1.g, a, sizeof(a));
    memcpy(G1.limite, lim, sizeof(lim));
    creatEdges(G1);
    regrouper(G1);
    /*
    for(int i=0;i<N;i++){
      cout<<G1.sousG[0].sommets[i]<<endl;
      //cout<<G1.sousG[0].edges[i].src<<","<<G1.sousG[0].edges[i].dest<<endl;
    }
    */
    //cout<<G1.sousG[0].connexion.size()<<endl;
    vector<Edge>::iterator iter;
    for (iter = G1.sousG[0].connexion.begin(); iter != G1.sousG[0].connexion.end(); ++iter)
        cout << iter->src << "," << iter->dest << endl;
    for (iter = G1.sousG[1].connexion.begin(); iter != G1.sousG[1].connexion.end(); ++iter)
        cout << iter->src << "," << iter->dest << endl;

    return 1;
}

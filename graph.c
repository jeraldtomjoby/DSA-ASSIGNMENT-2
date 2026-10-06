/* Q6: Social network graph - Adjacency Matrix vs Adjacency List
   BFS, DFS, vertex search and edge checking with operation counts */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 20

int n, m;
char label[MAX];
int mat[MAX][MAX];
typedef struct Node { int v; struct Node *next; } Node;
Node *adj[MAX];

int idx(char c){ for(int i=0;i<n;i++) if(label[i]==c) return i; return -1; }

/* insert at tail so neighbours stay in input order; we insert in sorted order of vertex */
void addList(int u,int v){
    Node *x=malloc(sizeof(Node)); x->v=v; x->next=NULL;
    if(!adj[u] || adj[u]->v > v){ x->next=adj[u]; adj[u]=x; return; }
    Node *p=adj[u];
    while(p->next && p->next->v < v) p=p->next;
    x->next=p->next; p->next=x;
}
void addEdge(int u,int v){ mat[u][v]=mat[v][u]=1; addList(u,v); addList(v,u); }

void printMatrix(){
    printf("    ");
    for(int i=0;i<n;i++) printf("%c ",label[i]);
    printf("\n");
    for(int i=0;i<n;i++){
        printf("  %c ",label[i]);
        for(int j=0;j<n;j++) printf("%d ",mat[i][j]);
        printf("\n");
    }
}
void printList(){
    for(int i=0;i<n;i++){
        printf("  %c -> ",label[i]);
        for(Node *p=adj[i];p;p=p->next) printf("%c ",label[p->v]);
        printf("\n");
    }
}

/* ---------- BFS ---------- */
void bfsMatrix(int s,long *ops){
    int vis[MAX]={0},q[MAX],f=0,r=0; *ops=0;
    vis[s]=1; q[r++]=s;
    printf("  %-6s %-14s %-22s %s\n","Step","Dequeued","Newly enqueued","Queue after");
    int step=1;
    while(f<r){
        int u=q[f++]; char nw[40]="";
        for(int v=0;v<n;v++){
            (*ops)++;                       /* one matrix cell inspected */
            if(mat[u][v] && !vis[v]){ vis[v]=1; q[r++]=v; char t[3]={label[v],' ',0}; strcat(nw,t); }
        }
        char qs[40]=""; for(int i=f;i<r;i++){ char t[3]={label[q[i]],' ',0}; strcat(qs,t);}
        printf("  %-6d %-14c %-22s [%s]\n",step++,label[u],nw[0]?nw:"-",qs);
    }
}
void bfsList(int s,long *ops){
    int vis[MAX]={0},q[MAX],f=0,r=0; *ops=0;
    vis[s]=1; q[r++]=s;
    printf("  %-6s %-14s %-22s %s\n","Step","Dequeued","Newly enqueued","Queue after");
    int step=1;
    while(f<r){
        int u=q[f++]; char nw[40]="";
        for(Node *p=adj[u];p;p=p->next){
            (*ops)++;                       /* one list node inspected */
            if(!vis[p->v]){ vis[p->v]=1; q[r++]=p->v; char t[3]={label[p->v],' ',0}; strcat(nw,t); }
        }
        char qs[40]=""; for(int i=f;i<r;i++){ char t[3]={label[q[i]],' ',0}; strcat(qs,t);}
        printf("  %-6d %-14c %-22s [%s]\n",step++,label[u],nw[0]?nw:"-",qs);
    }
}

/* ---------- DFS ---------- */
int vis[MAX]; long dops; int dstep;
void dfsMatrix(int u,int depth){
    vis[u]=1;
    printf("  %-6d %-10c %-8d visit %c\n",dstep++,label[u],depth,label[u]);
    for(int v=0;v<n;v++){ dops++; if(mat[u][v] && !vis[v]) dfsMatrix(v,depth+1); }
}
void dfsList(int u,int depth){
    vis[u]=1;
    printf("  %-6d %-10c %-8d visit %c\n",dstep++,label[u],depth,label[u]);
    for(Node *p=adj[u];p;p=p->next){ dops++; if(!vis[p->v]) dfsList(p->v,depth+1); }
}

/* ---------- Vertex search (BFS-based locate from A) ---------- */
int searchMatrix(int s,char target,long *ops){
    int vs[MAX]={0},q[MAX],f=0,r=0; *ops=0; vs[s]=1; q[r++]=s;
    while(f<r){
        int u=q[f++]; (*ops)++;                 /* compare label of dequeued vertex */
        if(label[u]==target) return 1;
        for(int v=0;v<n;v++){ (*ops)++; if(mat[u][v] && !vs[v]){ vs[v]=1; q[r++]=v; } }
    }
    return 0;
}
int searchList(int s,char target,long *ops){
    int vs[MAX]={0},q[MAX],f=0,r=0; *ops=0; vs[s]=1; q[r++]=s;
    while(f<r){
        int u=q[f++]; (*ops)++;
        if(label[u]==target) return 1;
        for(Node *p=adj[u];p;p=p->next){ (*ops)++; if(!vs[p->v]){ vs[p->v]=1; q[r++]=p->v; } }
    }
    return 0;
}

/* ---------- Edge check ---------- */
int edgeMatrix(int u,int v,long *ops){ *ops=1; return mat[u][v]; }
int edgeList(int u,int v,long *ops){
    *ops=0;
    for(Node *p=adj[u];p;p=p->next){ (*ops)++; if(p->v==v) return 1; }
    return 0;
}

int main(){
    FILE *fp=fopen("input.txt","r");
    if(!fp){ printf("input.txt not found\n"); return 1; }
    fscanf(fp,"%d",&n);
    for(int i=0;i<n;i++) fscanf(fp," %c",&label[i]);
    fscanf(fp,"%d",&m);
    for(int i=0;i<m;i++){ char a,b; fscanf(fp," %c %c",&a,&b); addEdge(idx(a),idx(b)); }
    fclose(fp);

    printf("===== Q6 : SOCIAL NETWORK GRAPH =====\n");
    printf("Vertices = %d, Edges = %d\n\n",n,m);
    printf("--- (a) Adjacency Matrix ---\n"); printMatrix();
    printf("\n--- (a) Adjacency List ---\n"); printList();

    long ops; int s=idx('A');
    printf("\n=== BFS from A : Adjacency Matrix ===\n"); bfsMatrix(s,&ops);
    printf("  Operations (matrix cells inspected) = %ld\n",ops);
    printf("\n=== BFS from A : Adjacency List ===\n"); bfsList(s,&ops);
    printf("  Operations (list nodes inspected)   = %ld\n",ops);

    printf("\n=== DFS from A : Adjacency Matrix ===\n");
    printf("  %-6s %-10s %-8s %s\n","Step","Vertex","Depth","Action");
    memset(vis,0,sizeof vis); dops=0; dstep=1; dfsMatrix(s,0);
    printf("  Operations (matrix cells inspected) = %ld\n",dops);
    printf("\n=== DFS from A : Adjacency List ===\n");
    printf("  %-6s %-10s %-8s %s\n","Step","Vertex","Depth","Action");
    memset(vis,0,sizeof vis); dops=0; dstep=1; dfsList(s,0);
    printf("  Operations (list nodes inspected)   = %ld\n",dops);

    printf("\n--- (b) Vertex search (start at A, BFS order) ---\n");
    printf("  %-8s %-10s %-16s %-16s\n","Target","Found?","Matrix ops","List ops");
    char targets[]={'A','D','F','G'};
    for(int i=0;i<4;i++){
        long om,ol; int fm=searchMatrix(s,targets[i],&om), fl=searchList(s,targets[i],&ol);
        printf("  %-8c %-10s %-16ld %-16ld\n",targets[i],(fm&&fl)?"Yes":"No",om,ol);
    }

    printf("\n--- (b) Edge check operations ---\n");
    printf("  %-8s %-10s %-16s %-16s\n","Edge","Exists?","Matrix ops","List ops");
    char eu[]={'A','B','C','D','A'}, ev[]={'B','E','F','F','F'};
    for(int i=0;i<5;i++){
        long om,ol; int a=edgeMatrix(idx(eu[i]),idx(ev[i]),&om), b=edgeList(idx(eu[i]),idx(ev[i]),&ol);
        printf("  %c-%-6c %-10s %-16ld %-16ld\n",eu[i],ev[i],(a&&b)?"Yes":"No",om,ol);
    }

    printf("\n--- Space ---\n");
    printf("  Matrix : %d x %d = %d cells (%zu bytes as int)\n",n,n,n*n,sizeof(int)*n*n);
    printf("  List   : %d head pointers + %d nodes (2E) = %d entries\n",n,2*m,n+2*m);
    return 0;
}

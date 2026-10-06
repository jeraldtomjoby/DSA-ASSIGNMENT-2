# DSA Assignment 2 – Question 6: Social Network Graph
**Subject:** PCCST303 Data Structures and Algorithms  |  **Topic:** Adjacency Matrix vs Adjacency List

## Repository contents
| File | Purpose |
|---|---|
| `graph.c` | Source code (C) – both representations, BFS, DFS, vertex search, edge check |
| `input.txt` | Input data (6 vertices, 6 edges: A–B, A–C, B–D, B–E, C–F, E–F) |
| `output.txt` | Actual program output |
| `README.md` | Trace tables, complexity analysis, comparison table, conclusion |

Run: `gcc graph.c -o graph && ./graph` (keep `input.txt` in the same folder).

Neighbours are always visited in alphabetical order, so both representations give identical traversals.

## (a) Representations
Adjacency matrix (symmetric, undirected):

|   | A | B | C | D | E | F |
|---|---|---|---|---|---|---|
| A | 0 | 1 | 1 | 0 | 0 | 0 |
| B | 1 | 0 | 0 | 1 | 1 | 0 |
| C | 1 | 0 | 0 | 0 | 0 | 1 |
| D | 0 | 1 | 0 | 0 | 0 | 0 |
| E | 0 | 1 | 0 | 0 | 0 | 1 |
| F | 0 | 0 | 1 | 0 | 1 | 0 |

Adjacency list: `A→B,C` `B→A,D,E` `C→A,F` `D→B` `E→B,F` `F→C,E`

### BFS trace (start A) – queue-based
| Step | Dequeued | Newly enqueued | Queue after |
|---|---|---|---|
| 1 | A | B, C | [B C] |
| 2 | B | D, E | [C D E] |
| 3 | C | F | [D E F] |
| 4 | D | – | [E F] |
| 5 | E | – | [F] |
| 6 | F | – | [ ] |

**BFS order: A B C D E F**

### DFS trace (start A) – recursion/stack-based
| Step | Vertex visited | Recursion depth | Note |
|---|---|---|---|
| 1 | A | 0 | start |
| 2 | B | 1 | first neighbour of A |
| 3 | D | 2 | leaf, backtrack to B |
| 4 | E | 2 | next neighbour of B |
| 5 | F | 3 | neighbour of E |
| 6 | C | 4 | neighbour of F; A already visited, done |

**DFS order: A B D E F C**

Operations during traversal: Matrix = 36 cell checks (n² = 36) for both BFS and DFS; List = 12 node checks (2E = 12) for both.

## (b) Vertex search and edge check (counted operations)
Vertex search starts at A and explores in BFS order; one operation = one label comparison, one matrix cell inspected or one list node inspected.

| Target | Found | Matrix ops | List ops |
|---|---|---|---|
| A | Yes | 1 | 1 |
| D | Yes | 22 | 11 |
| F | Yes | 36 | 16 |
| G (absent) | No | 42 | 18 |

Edge existence check:

| Edge | Exists | Matrix ops | List ops |
|---|---|---|---|
| A–B | Yes | 1 | 1 |
| B–E | Yes | 1 | 3 |
| C–F | Yes | 1 | 2 |
| D–F | No | 1 | 1 |
| A–F | No | 1 | 2 |

## (c) Complexity analysis (V = vertices, E = edges)
| Criterion | Adjacency Matrix | Adjacency List | Observed (V=6, E=6) |
|---|---|---|---|
| Space | O(V²) | O(V + E) | 36 cells vs 18 entries (6 heads + 12 nodes) |
| BFS / DFS time | O(V²) | O(V + E) | 36 vs 12 inspections |
| Vertex search (worst case) | O(V²) | O(V + E) | 42 vs 18 (absent vertex G) |
| Edge check (u,v) | O(1) | O(deg(u)) | 1 vs 1–3 |
| Find all neighbours of u | O(V) | O(deg(u)) | 6 vs 1–3 |
| Add edge | O(1) | O(1) at head (O(deg) if kept sorted, as here) | – |
| Auxiliary space for BFS/DFS | O(V) | O(V) | queue / visited / recursion stack |

Notes:
- The matrix scans a full row of V cells for every vertex, even though most cells are 0. The list only touches real edges.
- Every vertex has degree at most 3 here, so the graph is sparse. The matrix has 12 ones out of 36 cells (33% density), and the gap widens as the network grows.
- The matrix wins only on single edge lookup (always 1 operation). The list needs up to deg(u).
- Both representations give the same traversal order and the same result, so only cost differs.

## Comparison table
| Feature | Matrix | List | Better for sparse network |
|---|---|---|---|
| Memory | 36 cells | 18 entries | List |
| BFS/DFS cost | 36 | 12 | List |
| Vertex search cost (absent) | 42 | 18 | List |
| Edge check | 1 | ≤ 3 | Matrix (slightly) |
| Neighbour enumeration | Scan V cells | Only real neighbours | List |
| Implementation | Simple | Needs pointers | Matrix |

## Final conclusion
For a sparse social network (E is far smaller than V²), the **adjacency list is the more suitable representation**. The execution results show it used 3× fewer operations for BFS and DFS (12 vs 36), 2–3× fewer for vertex search (e.g. 18 vs 42 when the vertex is absent), and half the memory (18 vs 36 entries). Its O(V+E) bounds keep traversal and storage proportional to the actual number of friendships. The matrix grows as O(V²) regardless of how few connections exist, which becomes impractical for millions of users. The matrix's only advantage is O(1) edge checking, which does not outweigh these costs in a sparse network. A matrix is better suited to dense graphs or applications that mostly check "are X and Y friends?".

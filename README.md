# 🚗 Shortest Path Finder using Dijkstra's Algorithm

This C program finds the shortest path between two places using Dijkstra's algorithm. It reads an adjacency matrix and place names from a CSV file and displays the path with distance and color-coded segments based on distance ranges.

---

## ✨ Features

✅ Reads place names and distance matrix from a CSV file  
✅ Uses Dijkstra's algorithm to calculate the shortest path  
✅ Outputs color-coded paths:
- 🟢 Green: <10 km
- 🟡 Yellow: 10–19 km
- 🔵 Cyan: 20–49 km
- 🔴 Red: 50+ km  
✅ Clean terminal output with ANSI color codes

---

## 🗂️ File Structure

- `main.c` — Source code
- `places.csv` — CSV file containing place names and adjacency matrix

**Example `places.csv` format:**

```
A,B,C,D
0,10,0,30
10,0,50,0
0,50,0,20
30,0,20,0
```

---

## 📦 How to Compile

Use `gcc` or any C compiler:

```bash
gcc main.c -o shortest_path
```

---

## ▶️ How to Run

```bash
./shortest_path
```

Follow the prompts:

- Select the start and end locations by typing their names.

**Example interaction:**

```
Available places:
A, B, C, D
Enter the starting place: A
Enter the destination place: D

Shortest path from A to D:
A --(30km)--> D
Total Distance: 30km
```

---

## 🛠 Dependencies

- Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `limits.h`, `ctype.h`)

No external libraries required.

---

## 🧹 Notes

- Leading/trailing spaces in input are trimmed.
- A distance of 0 means no direct path (unless it's the same node).

---

## 📜 License

This project is open-source and available under the MIT License.

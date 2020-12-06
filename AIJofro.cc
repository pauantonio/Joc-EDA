#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Jofro


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  const vector<Dir> dirs = {Up,Down,Left,Right};

  int num_barricades = 0;

  bool evaluate_conditions (const int& id, const Pos& p) {
    if (is_day()) {
      if (citizen(id).type == Builder) return (citizen(id).life < builder_ini_life() and cell(p).bonus == Food) or (cell(p).bonus == Money);

      else return (cell(p).weapon != NoWeapon and cell(p).weapon > citizen(id).weapon) or (citizen(id).life < warrior_ini_life() and cell(p).bonus == Food) or (cell(p).bonus == Money);
    }

    else {
      if (citizen(id).type == Builder) return (citizen(id).life < builder_ini_life() and cell(p).bonus == Food) or (cell(p).bonus == Money) /*or (cell(p).b_owner == me())*/;

      else return (cell(p).id != -1 and citizen(cell(p).id).player != me() and ((citizen(cell(p).id).type == Builder) or (citizen(cell(p).id).weapon < citizen(id).weapon) or (citizen(cell(p).id).life < citizen(id).life))); 
    }
  }

  void bfs_search (const int& id) {
    Pos p = citizen(id).pos;
    set<Pos> visited_cells;
    visited_cells.emplace(p);
    queue<pair<pair<Pos, Dir>, int>> to_visit_cells;

    for (Dir d : dirs) {
      Pos new_pos = p + d;
      if (visited_cells.find(new_pos) == visited_cells.end() and pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
        to_visit_cells.push(make_pair(make_pair(new_pos, d), 1));
      }
    }

    while(to_visit_cells.size() > 0) {
      Pos possible_cell = to_visit_cells.front().first.first;
      Dir possible_dir = to_visit_cells.front().first.second;
      int cont = to_visit_cells.front().second;
      to_visit_cells.pop();
      visited_cells.emplace(possible_cell);

      if (cont >= 15) {
        Dir random_dir = dirs[random(0,3)];
        Pos new_pos = p + random_dir;
        if (pos_ok(new_pos) and cell(new_pos).type == Street) {
          move(id, random_dir);
          return;
        }
      }

      if (is_day() and citizen(id).type == Builder and num_barricades < max_num_barricades() and random(1, 5) == 1) {
        Dir random_dir = dirs[random(0,3)];
        Pos new_pos = p + random_dir;
        if (pos_ok(new_pos) and cell(new_pos).is_empty()) {
          build(id, random_dir);
          return;
        }
      }

      if (evaluate_conditions(id, possible_cell)) {
        move(id, possible_dir);
        return;
      }

      for (Dir d : dirs) {
        Pos new_pos = possible_cell + d;
        if (visited_cells.find(new_pos) == visited_cells.end() and pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
          to_visit_cells.push(make_pair(make_pair(new_pos, possible_dir), cont++));
        }
      }
    }
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    vector<int> b = builders(me());         // BUILDERS IDs
    vector<int> w = warriors(me());         // WARRIORS IDs
    num_barricades = 0;
    for (int i = 0; i < b.size(); ++i) num_barricades += barricades(b[i]).size();
    
    for (int id : b) bfs_search(id);
    for (int id : w) bfs_search(id);
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

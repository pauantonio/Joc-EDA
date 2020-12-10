#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Jofro3


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

  Dir oposite (const Dir& d) {
    if (d == Up) return Down;
    else if (d == Right) return Left;
    else if (d == Down) return Up;
    else return Right;
  }

  void day_builder (const int& id) {
    Pos p = citizen(id).pos;
    set<Pos> visited_cells;
    visited_cells.emplace(p);
    queue<pair<pair<Pos, Dir>, int>> to_visit_cells;

    for (Dir d : dirs) {
      Pos new_pos = p + d;
      if (pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
        to_visit_cells.push(make_pair(make_pair(new_pos, d), 1));

        if (cell(p).b_owner == -1 and num_barricades < max_num_barricades() and pos_ok(new_pos) and cell(new_pos).is_empty()) {
          build(id, d);
          ++num_barricades;
          return;
        }
      
        else if (cell(p).b_owner == -1 and cell(new_pos).b_owner == me() and cell(new_pos).resistance < barricade_max_resistance()) {
          build(id, d);
          return;
        }
      }
    }

    while(to_visit_cells.size() > 0) {
      Pos possible_cell = to_visit_cells.front().first.first;
      Dir possible_dir = to_visit_cells.front().first.second;
      int cont = to_visit_cells.front().second;
      to_visit_cells.pop();
      visited_cells.emplace(possible_cell);
/*
      if (cont >= 15) {
        Dir random_dir = dirs[random(0,3)];
        Pos new_pos = p + random_dir;
        if (pos_ok(new_pos) and cell(new_pos).type == Street) {
          move(id, random_dir);
          return;
        }
      }*/

      if ((cell(possible_cell).bonus == Money) or (citizen(id).life < builder_ini_life() and cell(possible_cell).bonus == Food)/* or (cell(possible_cell).weapon != NoWeapon)*/) {
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

  void day_warrior (const int& id) {
    Pos p = citizen(id).pos;
    set<Pos> visited_cells;
    visited_cells.emplace(p);
    queue<pair<pair<Pos, Dir>, int>> to_visit_cells;

    for (Dir d : dirs) {
      Pos new_pos = p + d;
      if (pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
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

      if ((cell(possible_cell).bonus == Money) or (cell(possible_cell).weapon != NoWeapon and cell(possible_cell).weapon > citizen(id).weapon) or (citizen(id).life < warrior_ini_life() and cell(possible_cell).bonus == Food) or (cell(possible_cell).weapon != NoWeapon)) {
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

  void night_builder (const int& id) {
    Pos p = citizen(id).pos;
    set<Pos> visited_cells;
    visited_cells.emplace(p);
    queue<pair<pair<Pos, Dir>, int>> to_visit_cells;

    for (Dir d : dirs) {
      Pos new_pos = p + d;
      if (pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
        to_visit_cells.push(make_pair(make_pair(new_pos, d), 1));

        if (cell(p).b_owner == me() and cell(new_pos).id != -1 and weapon_strength_demolish(citizen(cell(new_pos).id).weapon) > cell(p).resistance) {
          Dir oposite_dir = oposite(d);
          new_pos = p + oposite_dir;
          if (pos_ok(new_pos) and cell(new_pos).type == Street) {
            move(id, oposite_dir);
            return;
          }
        }

        else if ((cell(new_pos).id != -1 and citizen(cell(new_pos).id).type == Builder and citizen(cell(new_pos).id).life < citizen(id).life) or (cell(new_pos).bonus == Money) or (citizen(id).life < builder_ini_life() and cell(new_pos).bonus == Food)) {
          move(id, d);
          return;
        }
      }
    }

    if (cell(p).b_owner == me()) return;

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

      if ((cell(possible_cell).b_owner == me() and cell(possible_cell).id == -1) or (cell(possible_cell).bonus == Money) or (citizen(id).life < builder_ini_life() and cell(possible_cell).bonus == Food)) {
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

  void night_warrior (const int& id) {
    Pos p = citizen(id).pos;
    set<Pos> visited_cells;
    visited_cells.emplace(p);
    queue<pair<pair<Pos, Dir>, int>> to_visit_cells;

    for (Dir d : dirs) {
      Pos new_pos = p + d;
      if (pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
        to_visit_cells.push(make_pair(make_pair(new_pos, d), 1));

        if ((cell(new_pos).id != -1) and (citizen(cell(new_pos).id).player != me()) and (citizen(cell(new_pos).id).type == Warrior) and ((citizen(cell(new_pos).id).weapon > citizen(id).weapon) or ((citizen(cell(new_pos).id).weapon == citizen(id).weapon) and (citizen(cell(new_pos).id).life > citizen(id).life)))) {
          Dir oposite_dir = oposite(d);
          new_pos = p + oposite_dir;
          if (pos_ok(new_pos) and cell(new_pos).type == Street) {
            move(id, oposite_dir);
            return;
          }
        }
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

      if ((cell(possible_cell).id != -1 and citizen(cell(possible_cell).id).player != me() and ((citizen(cell(possible_cell).id).type == Builder) or (citizen(cell(possible_cell).id).weapon < citizen(id).weapon) or (citizen(cell(possible_cell).id).life < citizen(id).life))) or (cell(possible_cell).weapon != NoWeapon and cell(possible_cell).weapon > citizen(id).weapon) or (cell(possible_cell).bonus == Money)) {
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
    num_barricades = barricades(me()).size();

    if (is_day()) {
      for (int id : b) day_builder(id);

      for (int id : w) day_warrior(id);
    }

    else {
      for (int id : b) night_builder(id);

      for (int id : w) night_warrior(id);
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

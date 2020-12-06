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
/*
  void search (const int& id) {
    Pos p = citizen(id).pos;
    bool money_nearby = false;
    Dir money_dir;
    
    for (int i = 0; !money_nearby and i < 4; ++i) {
      Dir d = dirs[i];
      Pos new_pos = p+d;
      if (pos_ok(new_pos) and cell(new_pos).bonus == Money) {
        money_nearby = true;
        money_dir = d;
      }
  	}

    if (money_nearby) move(id, money_dir);
    else {
      for (int i = 0; !money_nearby and i < 4; ++i) {
        Dir d = dirs[i];
        Pos new_pos = p+d;

        if (d == Up) {
          const vector<Dir> dirs2 = {Up,Right};
          for (int j = 0; !money_nearby and j < 2; ++j) {
            Pos new_pos2 = new_pos + dirs2[j];
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }

        else if (d == Right) {
          const vector<Dir> dirs2 = {Right,Down};
          for (int j = 0; !money_nearby and j < 2; ++j) {
            Pos new_pos2 = new_pos + dirs2[j];
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }

        else if (d == Down) {
          const vector<Dir> dirs2 = {Down,Left};
          for (int j = 0; !money_nearby and j < 2; ++j) {
            Pos new_pos2 = new_pos + dirs2[j];
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }

        else {
          const vector<Dir> dirs2 = {Left,Up};
          for (int j = 0; !money_nearby and j < 2; ++j) {
            Pos new_pos2 = new_pos + dirs2[j];
            if (pos_ok(new_pos2) and cell(new_pos2).bonus == Money) {
              money_nearby = true;
              money_dir = d;
            }
          }
        }
      }
      
      if (money_nearby and cell(p+money_dir).type == Street) move(id, money_dir);
      else {
        Dir random_dir = dirs[random(0,3)];
	      if (pos_ok(p+random_dir) and cell(p+random_dir).type == Street) move(id,random_dir);
        else {
          Dir random_dir2 = dirs[random(0,3)];
	        if (pos_ok(p+random_dir2) and cell(p+random_dir2).type == Street) move(id,random_dir2);
        }
      }
    }
  }*/

  void money_search (const int& id) {
    Pos p = citizen(id).pos;
    set<Pos> visited_cells;
    visited_cells.emplace(p);
    queue<pair<Pos, Dir>> to_visit_cells;
    Dir money_dir;

    for (Dir d : dirs) {
      Pos new_pos = p + d;
      if (visited_cells.find(new_pos) == visited_cells.end() and pos_ok(new_pos) and cell(new_pos).type == Street) {
        to_visit_cells.push(make_pair(new_pos, d));
      }
    }

    while(to_visit_cells.size() > 0) {
      Pos possible_cell = to_visit_cells.front().first;
      Dir possible_dir = to_visit_cells.front().second;
      to_visit_cells.pop();
      visited_cells.emplace(possible_cell);

      if (cell(possible_cell).bonus == Money) {
        money_dir = possible_dir;
        break;
      }

      for (Dir d : dirs) {
        Pos new_pos = possible_cell + d;
        if (visited_cells.find(new_pos) == visited_cells.end() and pos_ok(new_pos) and cell(new_pos).type == Street) {
          to_visit_cells.emplace(new_pos, possible_dir);
        }
      }
    }

    move(id, money_dir);
  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    vector<int> b = builders(me());         // BUILDERS IDs
    vector<int> w = warriors(me());         // WARRIORS IDs
    /*
    if (is_day()) {                         // THE CURRENT ROUND IS DAY
      for (int id : b) {

      }
    }

    else {                                  // THE CURRENT ROUND IS NIGHT

    }*/
    for (int id : b) money_search(id);
    for (int id : w) money_search(id);
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

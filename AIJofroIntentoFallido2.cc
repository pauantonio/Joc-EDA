#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME JoftoIntentoFallido2


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

  void day_builder(const int& id) {
    Pos p = citizen(id).pos;
    vector<vector<bool>> visited (board_rows(), vector<bool> (board_cols(), false));
    visited[p.i][p.j] = true;
    queue<Pos> to_visit;

    for (Dir d : dirs) {
      Pos possible_pos = p + d;

      if (barricades(me()).size() < uint(max_num_barricades()) and pos_ok(possible_pos) and cell(possible_pos).type == Street and cell(possible_pos).is_empty() and cell(p).b_owner == -1) {
        build(id, d);
        return;
      }

      if (pos_ok(possible_pos) and cell(possible_pos).type == Street and cell(possible_pos).b_owner == me() and cell(possible_pos).resistance < barricade_max_resistance() and cell(p).b_owner == -1) {
        build(id, d);
        return;
      }

      if (pos_ok(new_pos) and cell(new_pos).type == Street and (cell(new_pos).b_owner == -1 or cell(new_pos).b_owner == me())) {
        to_visit.push(possible_pos);
      }
    }

    while (not to_visit.empty()) {
      
    }
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if (is_day()) {
      for (int id : builders(me())) day_builder(id);

      for (int id : warriors(me())) day_builder(id);
    }

    else {
      for (int id : builders(me())) day_builder(id);

      for (int id : warriors(me())) day_builder(id);
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

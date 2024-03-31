

// don't change those includes
#include "User.h"

#include <utility>
#include "RecommendationSystem.h"

#define MAXRATEVAL 10
#define MINRATEVAL 1

// implement your cpp code here
User::User (std::string &name, rank_map &ranks,
            std::shared_ptr<RecommendationSystem> rs) : name (name), ranks
    (ranks), rs (rs) {}

std::string User::get_name () const {
  return name;
}

const rank_map &User::get_ranks () const {
  return ranks;
}

void
User::add_movie_to_rs (const std::string &name, int year, const std::vector<double> &features, double rate) {
  // TODO: check if rate check is needed
//  if (rate < MINRATEVAL || rate > MAXRATEVAL) {
//    throw std::out_of_range ("The rate is out of range\n");
//  }
  rs->add_movie (name, year, features);
  ranks[rs->get_movie (name, year)] = rate;
}

sp_movie User::get_recommendation_by_content () const {
  return rs->recommend_by_content (*this);
}

sp_movie User::get_recommendation_by_cf (int k) const {
  return rs->recommend_by_cf (*this, k);
}

double
User::get_prediction_score_for_movie (const std::string &name, int year, int k) const {
  return rs->predict_movie_score (*this, rs->get_movie (name, year), k);
}

std::ostream &operator<< (std::ostream &os, const User &u) {
  os << "name: " << u.name << std::endl << *(u.rs) << std::endl;
  return os;
}



#include "RecommendationSystem.h"

bool
RecommendationSystem::pair_comparator (std::pair<sp_movie, double> &v1,
                                       std::pair<sp_movie, double> &v2) {
  return (v2.second) < (v1.second);
}

bool
RecommendationSystem::mov_comparator (sp_movie m1, sp_movie m2) {
  return *m1 < *m2;
}

RecommendationSystem::RecommendationSystem () : movies (mov_comparator) {}

sp_movie
RecommendationSystem::add_movie (const std::string &name, int year, const
std::vector<double>
&features) {
  sp_movie movie = std::make_shared<Movie> (name, year);
  movies[movie] = features;
  return movie;
}

//sp_movie
//RecommendationSystem::recommend_by_content (const User &user) {
//  // first step - normalize the ranks
//  rank_map u_ranks = user.get_ranks ();
//  if (u_ranks.empty ()) {
//    return nullptr;
//  }
//  rank_map norm_ranks = normalize_ranks (u_ranks);
//// second step - create vector of preferences
//  std::vector<double> user_preferences = std::vector<double> (movies.begin ()
//                                                                  ->second
//                                                                  .size (), 0.0);
//  std::vector<double> pref_vec = create_pref_vec (norm_ranks, user_preferences);
//  // third step - similarity calculation
//  sp_movie prediction = nullptr;
//  sp_movie res = find_prediction_by_content (norm_ranks, pref_vec, prediction);
//  return res;
//}

sp_movie
RecommendationSystem::recommend_by_content (const User &user) {
  // step 1
  rank_map u_ranks = user.get_ranks ();
  if (u_ranks.empty ()) {
    return nullptr;
  }
  double sum = 0;
  for (const auto &p: u_ranks) {
    sum += p.second;
  }
  double avg = sum / u_ranks.size ();
  for (auto &p: u_ranks) { p.second -= avg; }
  // step 2
  std::vector<double> preferences (movies.begin ()->second.size (), 0);
  for (const auto &rank: u_ranks) {
    const auto &movie = movies.find (rank.first);
    if (movie != movies.end ()) {
      const std::vector<double> &features = movie->second;
      for (unsigned int k = 0; k < features.size (); k++) {
        preferences[k] += rank.second * features[k];
      }
    }
  }
  // step 3
  sp_movie best_movie = nullptr;
  double res = -1;
  for (const auto &mov: movies) {
    if (u_ranks.find (mov.first) == u_ranks.end ()) {
      const std::vector<double> &features = mov.second;
      double curr_score = similarity_check (preferences, features);
      if (curr_score > res || best_movie == nullptr) {
        best_movie = mov.first;
        res = curr_score;
      }
    }
  }
  return best_movie;
}

sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k) {
  // step 1 - find vector of k most similar movies
  rank_map predictions = rank_map (0, sp_movie_hash, sp_movie_equal);
  rank_map u_ranks = user.get_ranks ();
  for (const auto &mov: movies) {
    if (u_ranks.find (mov.first) == u_ranks.end ()) {
      predictions[mov.first] = predict_movie_score (user, mov
          .first, k);
    }
  }
  // step 2 - find best movie
  sp_movie prediction = nullptr;
  sp_movie res = find_prediction_by_cf (predictions, prediction);
  return res;

}

double
RecommendationSystem::predict_movie_score (const User &user, const sp_movie &movie, int k) {
  rank_map u_ranks = user.get_ranks ();
//  if (ranks.empty ()) { return 0; }
  std::vector<double> movie_vec = movies[movie];
  std::vector<std::pair<sp_movie, double>> similar_ranks;
  for (const auto &mov: u_ranks) {
    const std::vector<double> &features = movies[mov.first];
    double similarity = similarity_check (movie_vec,
                                          features);
    similar_ranks.emplace_back (mov.first, similarity);
  }
  std::sort (similar_ranks.begin (), similar_ranks.end (), pair_comparator);
  double numerator = 0;
  double denominator = 0;
  for (int i = 0; i < k; i++) {
    const sp_movie &similar_movie = similar_ranks[i].first;
    double val = similar_ranks[i].second;
    if (u_ranks.find (similar_movie) != u_ranks.end ()) {
      numerator += u_ranks[similar_movie] * val;
      denominator += val;
    }
  }
  if (denominator == 0) {
    return 0;
  }
  return numerator / denominator;
}

sp_movie
RecommendationSystem::get_movie (const std::string &name, int year) const {
  sp_movie movie = std::make_shared<Movie> (name, year);
  auto k = movies.find (movie);
  auto mov = (k != movies.end ()) ? k->first : nullptr;
  return mov;
}

std::ostream &operator<< (std::ostream &os, const
RecommendationSystem &rs) {
  for (const auto &val: rs.movies) {
    os << *val.first;
  }
  return os;
}

// PRIVATE FUNCTIONS
rank_map &RecommendationSystem::normalize_ranks (rank_map &ranks) {
  unsigned long ranks_sum = 0;
  for (const auto &rank: ranks) {
    ranks_sum += (unsigned long) rank.second;
  }
  unsigned long average = ranks_sum / ranks.size ();
  for (auto &rank: ranks) {
    rank.second -= (double) average;
  }
  return ranks;
}

std::vector<double> &RecommendationSystem::create_pref_vec (rank_map &ranks,
                                                            std::vector<double> &vec) {
  for (auto &pair: ranks) {
    auto m = ranks.find (pair.first);
    if (m != ranks.end ()) {
      for (size_t j = 0; j < vec.size (); ++j) {
        vec[j] = pair.second * movies[pair.first][j];
      }
    }
  }
  return vec;
}

sp_movie &RecommendationSystem::find_prediction_by_content (rank_map &u_ranks,
                                                            std::vector<double> &prefs,
                                                            sp_movie &prediction) {
  double best_score = -1;
  for (const auto &mov: movies) {
    if (u_ranks.find (mov.first) == u_ranks.end ()) {
      const std::vector<double> &movie_features = mov.second;
      double movie_score = similarity_check (prefs, movie_features);
      if (movie_score > best_score || prediction == nullptr) {
        prediction = mov.first;
        best_score = movie_score;
      }
    }
  }
  return prediction;
}

double
RecommendationSystem::similarity_check (std::vector<double> &v1, const std::vector<double> &v2) {
  double sum = 0;
  double norm_1 = 0;
  double norm_2 = 0;

  for (size_t i = 0; i < v1.size (); i++) {
    sum += v1[i] * v2[i];
    norm_1 += v1[i] * v1[i];
    norm_2 += v2[i] * v2[i];
  }
  return sum / (std::sqrt (norm_1) * std::sqrt (norm_2));
}

sp_movie &RecommendationSystem::find_prediction_by_cf (rank_map
                                                       &predictions, sp_movie &best_movie) {
  double best_rank = -1;
  for (const auto &entry: predictions) {
    if (entry.second > best_rank || best_rank == -1) {
      best_movie = entry.first;
      best_rank = entry.second;
    }
  }
  return best_movie;
}






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
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  movies[new_movie] = features;
  return new_movie;
}

sp_movie
RecommendationSystem::recommend_by_content (const User &user) {
  /// first step - normalize the ranks
  rank_map u_ranks = user.get_ranks ();
  if (u_ranks.empty ()) {
    return nullptr;
  }
  double sum = std::accumulate (u_ranks.begin (), u_ranks.end (), 0.0,
                                [] (double acc, const auto &rank) {
                                    return acc + rank.second;
                                });
  double avg = sum / static_cast<double>(u_ranks.size ());
  for (auto &rank: u_ranks) {
    rank.second -= avg;
  }
  /// second step - create vector of preferences
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
  /// third step - similarity calculation
  sp_movie best_movie = nullptr;
  double max_similarity = -1;
  for (const auto &mov: movies) {
    if (u_ranks.find (mov.first) == u_ranks.end ()) {
      const std::vector<double> &features = mov.second;
      double movie_sim = similarity_check (preferences, features);
      if (movie_sim > max_similarity || best_movie == nullptr) {
        best_movie = mov.first;
        max_similarity = movie_sim;
      }
    }
  }
  /// return the most similar movie based on content recommendation
  return best_movie;
}

sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k) {
  /// first step - find vector of k most similar movies
  rank_map predictions = rank_map (0, sp_movie_hash, sp_movie_equal);
  rank_map u_ranks = user.get_ranks ();
  for (const auto &mov: movies) {
    if (u_ranks.find (mov.first) == u_ranks.end ()) {
      predictions[mov.first] = predict_movie_score (user,
                                                    mov.first,
                                                    k);
    }
  }
  ///second step - find best movie
  sp_movie best_movie = nullptr;
  double max_score = -1;
  for (const auto &p: predictions) {
    if (p.second > max_score || max_score == -1) {
      best_movie = p.first;
      max_score = p.second;
    }
  }
  /// return the best movie based on collaborative filtering algorithm
  return best_movie;
}

double
RecommendationSystem::predict_movie_score
    (const User &user, const sp_movie &movie, int k) {
  rank_map u_ranks = user.get_ranks ();
  std::vector<double> movie_vec = movies[movie];
  /// vector to store the movie with its similarity calculation
  std::vector<std::pair<sp_movie, double>> similar_ranks;
  std::transform (u_ranks.begin (), u_ranks.end (),
                  std::back_inserter (similar_ranks),
                  [&] (const auto &mov) {
                      const std::vector<double> &features = movies[mov.first];
                      return std::make_pair
                          (mov.first, similarity_check (movie_vec, features));
                  });
  /// sort the ranks for the similar movies
  std::sort (similar_ranks.begin (), similar_ranks.end (), pair_comparator);
  double numerator = 0;
  double denominator = 0;
  /// calculate prediction
  std::for_each (similar_ranks.begin (), similar_ranks.begin () + k,
                 [&] (const auto &pair) {
                     const sp_movie &similar_movie = pair.first;
                     double val = pair.second;
                     if (u_ranks.find (similar_movie) != u_ranks.end ()) {
                       numerator += u_ranks[similar_movie] * val;
                       denominator += val;
                     }
                 });
  if (denominator == 0) { // prevent division by zero
    return 0;
  }
  return numerator / denominator;
}

sp_movie
RecommendationSystem::get_movie (const std::string &name, int year) const {
  sp_movie movie = std::make_shared<Movie> (name, year);
  auto k = movies.find (movie);
  sp_movie mov = (k != movies.end ()) ? k->first : nullptr;
  return mov;
}

std::ostream &operator<< (std::ostream &os, const
RecommendationSystem &rs) {
  for (const auto &movie: rs.movies) {
    os << *movie.first;
  }
  return os;
}

/// PRIVATE FUNCTIONS

// the function that calculates the vector operation:
// v1 * v2 / (|v1| * |v2|)
double
RecommendationSystem::similarity_check
    (std::vector<double> &v1, const std::vector<double> &v2) {
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







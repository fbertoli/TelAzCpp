#include "largeNeighborhoodSearch.h"
#include "solution.h"
#include "configParameters.h"
#include <vector>
#include <cassert>
#include <ctime>
#include <fstream>
#include <numeric>
#include "utilities.h"



using namespace std;


LargeNeighborhoodSearch:: LargeNeighborhoodSearch(Solution &initial_solution, std::vector<Cost *> *costs, std::vector<SolutionOperator *> &destroy_methods, std::vector<SolutionOperator *> &repair_methods, std::string output_folder) :
        Metaheuristic(costs, output_folder),
        initial_solution_(initial_solution),
        best_solution_(initial_solution_.data_),
        current_solution_(initial_solution_.data_),
        best_feasible_solution_(initial_solution.data_),
        destroy_methods_(destroy_methods),
        repair_methods_(repair_methods),
        candidate_solution_(initial_solution.data_),
//        length_memory_(configParameter:LNS::length_memory),
//        segment_iterations_(configParameter:LNS::segment_iterations),
//        reactivity_(configParameter:LNS::reactivity),
//        sigma_1_(configParameter:LNS::sigma_1_),
//        sigma_2_(configParameter:LNS::sigma_2_),
//        sigma_3_(configParameter:LNS::sigma_3_),
//        max_iterations_(configParameter:LNS::max_iterations),
//        cost_memory_(configParameter:LNS::length_memory, 0.0),
//        cost_memory_components_(configParameter:LNS::length_memory, std::unordered_map<int, double>()),
//        max_reheats_(configParameter:SA::max_reheats),
//        end_temperature_(configParameter:SA::end_temperature),
//        cooling_rate_(configParameter:SA::cooling_rate),
//        initial_probability_(configParameter:SA::initial_probability),
//        accepted_deterioration_(configParameter:SA::accepted_deterioration),
        visited_solution_hashes_(),
        destroy_weights_(destroy_methods.size()),
        repair_weights_(repair_methods.size()),
        destroy_scores_(destroy_methods.size()),
        repair_scores_(repair_methods.size()),
        destroy_counters_(destroy_methods.size()),
        repair_counters_(repair_methods.size()),
        destroy_cumulative_probabilities_(int(destroy_methods.size()),0),
        repair_cumulative_probabilities_(int(repair_methods.size()),0),
        current_time_(),
        start_time_(),
        // statistics fields
        cost_evolution_(),
        cost_components_evolution_(),
        destroy_weights_evolution_(destroy_methods.size(), vector<double>()),
        repair_weights_evolution_(repair_methods.size(), vector<double>()),
        temperature_evolution_(),
        destroy_type_(),
        repair_type_()
{
    visited_solution_hashes_.clear();
    for (auto &cost : *costs_) {
        cost_components_evolution_[cost->id_] = vector<double>();
        cost_weights_[cost->id_] = 1;
        for (auto & el : cost_memory_components_)
            el[cost->id_] = 0;

    }

//    for (auto &method : repair_methods) {
//        method->metaheuristic_cost_weight_ = &cost_weights_;
//    }
}



/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::updateTemperature() {
    temperature_ *= cooling_rate_;
}

/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::computeInitialTemperature() {
//    // compute initial solution cost without outsourcing cost
//    double cost = 0;
//    for (auto & cost_source : *costs_)
//        if (cost_source->id_ != configParameter:cost_outsourcing_id and ((not single_day_modality_) or cost_source->id_ != configParameter:cost_district_violation_id)) {
//            cost += initial_solution_.cost_map_.back()[cost_source->id_];
//        }
//
//    initial_temperature_ = - accepted_deterioration_ * cost / log(initial_probability_);
}



/** ------------------------------------------------------------------------------------------------ */

bool LargeNeighborhoodSearch::stoppingCriterion() {
//    if (iterations_ >= max_iterations_){
//        cout << "  STOP! Max Iteration Reached." << endl;
//        return true;
//    }
//    if (temperature_ <= end_temperature_){
//        cout << "  STOP! End Temperature Reached." << endl;
//        return true;
//    }
//    if (configParameter:maximum_time > 0) {
//        current_time_ = clock();
//        if ((double(current_time_ - start_time_) / CLOCKS_PER_SEC) > configParameter:maximum_time) {
//            cout << "  STOP! Maximum Time Reached." << endl;
//            return true;
//        }
//    }
//    return false;
    return true;
}

/** ------------------------------------------------------------------------------------------------ */

bool LargeNeighborhoodSearch::acceptCandidateSolution() {
    return delta_ < threesold_delta_;
}


/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::updateWeights() {
    int i;
    double sum = 0;
    for (i=0; i < destroy_weights_.size(); ++i) {
        destroy_weights_[i] =
                destroy_weights_[i] * (1 - reactivity_) + ((destroy_counters_[i] > 0) ? (reactivity_ * destroy_scores_[i] / destroy_counters_[i]) : 0);
        sum += destroy_weights_[i];
    }

    destroy_cumulative_probabilities_[0] = destroy_weights_[0]/sum;
    for (i=1; i < destroy_weights_.size(); ++i)
        destroy_cumulative_probabilities_[i] = destroy_cumulative_probabilities_[i-1] +  destroy_weights_[i]/sum;

    sum = 0;
    for (i=0; i < repair_weights_.size(); ++i) {
        repair_weights_[i] =
                repair_weights_[i] * (1 - reactivity_) + ((repair_counters_[i] > 0) ? (reactivity_ * repair_scores_[i] / repair_counters_[i]) : 0);
        sum += repair_weights_[i];
    }

    repair_cumulative_probabilities_[0] = repair_weights_[0]/sum;
    for (i=1; i < repair_weights_.size(); ++i)
        repair_cumulative_probabilities_[i] = repair_cumulative_probabilities_[i-1] +  repair_weights_[i]/sum;


    // statistics
    for (i = 0; i < destroy_weights_.size(); ++i)
        destroy_weights_evolution_[i].push_back(destroy_weights_[i]);
    for (i = 0; i < repair_weights_.size(); ++i)
        repair_weights_evolution_[i].push_back(repair_weights_[i]);
}


/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::updateScores() {
//    if (candidate_solution_.cost_ < best_solution_.cost_)
//    {
//        destroy_scores_[selected_destroy_] += sigma_1_;
//        repair_scores_[selected_repair_] += sigma_1_;
//    }
//    else
//    {
//        size_t new_hash(candidate_solution_.computeHash());
//        if (acceptCandidateSolution() and
//            visited_solution_hashes_.find(new_hash) == visited_solution_hashes_.end())
//        {
//            visited_solution_hashes_.insert(new_hash);
//            if (delta_ < 0)
//            {
//                destroy_scores_[selected_destroy_] += sigma_2_;
//                repair_scores_[selected_repair_] += sigma_2_;
//            }
//            else
//            {
//                destroy_scores_[selected_destroy_] += sigma_3_;
//                repair_scores_[selected_repair_] += sigma_3_;
//            }
//        }
//    }
//
//    for (int i = 0; i < destroy_weights_.size(); ++i)
//        destroy_weights_evolution_[i].push_back(destroy_weights_[i] * (1 - reactivity_) + ((destroy_counters_[i] > 0) ? (reactivity_ * destroy_scores_[i] / destroy_counters_[i]) : 0));
//    for (int i = 0; i < repair_weights_.size(); ++i)
//        repair_weights_evolution_[i].push_back(repair_weights_[i] * (1 - reactivity_) +  ((repair_counters_[i] > 0) ? (reactivity_ * repair_scores_[i] / repair_counters_[i]) : 0));
}


/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::selectDestroyMethod() {
    double point(Random::continuous());
    for (int i = 0; i < destroy_cumulative_probabilities_.size(); ++i)
        if (point <= destroy_cumulative_probabilities_[i]) {
            destroy_type_.push_back(i);
            selected_destroy_ = i;
            ++destroy_counters_[i];
            return;
        }
}

/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::selectRepairMethod() {
    double point(Random::continuous());
    for (int i = 0; i < repair_cumulative_probabilities_.size(); ++i)
        if (point <= repair_cumulative_probabilities_[i]) {
            repair_type_.push_back(i);
            selected_repair_ = i;
            ++repair_counters_[i];
            return;
        }
}

/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::sampleMove() {
//    // copy solutin
//    candidate_solution_ = current_solution_;
//
//    // modify solution
//    selectDestroyMethod();
//
//    destroy_methods_[selected_destroy_]->destroy(&candidate_solution_, day_);
//
//    selectRepairMethod();
//    repair_methods_[selected_repair_]->repair(&candidate_solution_, day_);

}



/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::reheatTemperature() {
    temperature_ = initial_temperature_ * (1 - double(reheat_counter_)/(3 * max_reheats_));
    iterations_ = 0;
    if (reheat_counter_ > 0)
        max_iterations_ = int(max_iterations_ * 2.0/3.0);

}

/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::reset() {
//    temperature_ = initial_temperature_ ;
//    max_iterations_ =  configParameter:LNS::max_iterations;
//    iterations_ = 0;
//    reheat_counter_ = 0;
}


/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::setUpCosts() {
//    // look up for special costs
//    cost_districting = nullptr;
//    cost_clusterisation = nullptr;
//    for (auto &cost : *costs_) {
//        if (cost->id_ == configParameter:cost_district_violation_id) {
//            cost_districting = static_cast<CostDistrictingViolation *> (cost);
//            if (increase_cost_districting_during_search_) {
//                cost_weights_[cost_districting->id_] = 0;
//            }
//        }
//        if (cost->id_ == configParameter:cost_clusterisation_id) {
//            cost_clusterisation = static_cast<CostClusterisation *> (cost);
//            if (configParameter:clusterisation_increase_in_lns) {
//                cost_weights_[cost_clusterisation->id_] = 0;
//            }
//        }
//    }
}

/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::adjustCosts() {
//    if (increase_cost_districting_during_search_ and cost_districting) {
//        // linear
//        cost_weights_[cost_districting->id_] = double(iterations_) / true_max_iterations_;
//        // quadratic
////        cost_weights_[cost_districting->id_] =  pow(double(iterations_) / true_max_iterations_, 2);
//    }
//
//    if (configParameter:clusterisation_increase_in_lns and cost_clusterisation) {
//        cost_weights_[cost_clusterisation->id_] = double(iterations_) / true_max_iterations_;
//    }
//
//    // adjust cost memory
//    for (int i = 0; i < cost_memory_components_.size(); ++i) {
//        cost_memory_[i] = 0;
//        for (auto &cost : *costs_)
//            cost_memory_[i] += (cost_memory_components_[i][cost->id_] * cost_weights_[cost->id_]);
//    }
}

/** ------------------------------------------------------------------------------------------------ */

double LargeNeighborhoodSearch::computeWeightedCost(Solution &solution) {
//    solution.computeCost(*costs_);
//    double weighted_cost = 0;
//    for (auto &cost : *costs_) {
//        weighted_cost += (solution.cost_map_.back()[cost->id_] * cost_weights_[cost->id_]);
//    }
//    return weighted_cost;
return 0;
}

/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::run() {
//    // start measuring time
//    clock_t start_time_ = clock();
//
//    // set up costs
//    setUpCosts();
//
//    // initialize solutions and costs
//    initial_solution_cost_ = computeWeightedCost(initial_solution_);
//
//    current_solution_ = initial_solution_;
//    current_solution_cost_ = initial_solution_cost_;
//
//    initial_solution_.extractBestSolution(best_solution_);
//    best_solution_cost_ = initial_solution_cost_;
//
//    initial_solution_.extractBestSolution(real_best_solution);
//
//    best_feasible_solution_cost_ = numeric_limits<double>::max();
//    if (initial_solution_.checkCapacityFeasibility(day_) and initial_solution_.checkTimeFeasibility(day_)) {
//        initial_solution_.extractBestSolution(best_feasible_solution_);
//        best_feasible_solution_cost_ = initial_solution_cost_;
//    }
//
//    // set parameters
//    if (notify_clusterisation_) {
//        clusterisation_->newBestSolutionFound(&best_solution_, day_);
//    }
//    // initialize temperature and counters
//    computeInitialTemperature();
//    temperature_ = initial_temperature_;
//    true_max_iterations_ = min(max_iterations_,
//                               int(log(end_temperature_ / double(initial_temperature_)) / log(cooling_rate_)));
//    reheat_counter_ = 0;
//
//    // statistics
////    cost_evolution_.push_back(current_solution_.cost_);
////    for (auto &cost : *costs_)
////        cost_components_evolution_[cost->id_].push_back(current_solution_.cost_map_.back()[cost->id_]);
////    temperature_evolution_.push_back(initial_temperature_);
//
//
//    // start algorithm
//    while (reheat_counter_ < max_reheats_) {
//        reheatTemperature();
//        ++reheat_counter_;
//
//        // initialize weights and probabilities
//        fill(destroy_weights_.begin(), destroy_weights_.end(), 1 / (double(destroy_weights_.size())));
//        partial_sum(destroy_weights_.begin(), destroy_weights_.end(), destroy_cumulative_probabilities_.begin());
//        fill(repair_weights_.begin(), repair_weights_.end(), 1 / (double(repair_weights_.size())));
//        partial_sum(repair_weights_.begin(), repair_weights_.end(), repair_cumulative_probabilities_.begin());
//
//        // initialize cost memories
//        memory_lookup_ = 0;
//        fill(cost_memory_.begin(), cost_memory_.end(), current_solution_cost_);
//        for (auto &el : cost_memory_components_)
//            for (auto &cost: *costs_)
//                el[cost->id_] = current_solution_.cost_map_.back()[cost->id_];
//
//
//        // if we are using ditricting cost reorder the routes
//        if (cost_districting and not single_day_modality_) {
//            reorder_operator_.reorderRoutes(&initial_solution_);
//            if (increase_cost_districting_during_search_)
//                cost_weights_[cost_districting->id_] = 0;
//        }
//        // reset weight of cost clusterisation
//        if (cost_clusterisation and configParameter:clusterisation_increase_in_lns)
//            cost_weights_[cost_clusterisation->id_] = 0;
//
//        // statistics
////        for (int i = 0; i < destroy_weights_.size(); ++i)
////            destroy_weights_evolution_[i].push_back(destroy_weights_[i]);
////        for (int i = 0; i < repair_weights_.size(); ++i)
////            repair_weights_evolution_[i].push_back(repair_weights_[i]);
//
//
//        // LNS iteration -------
//        while (!stoppingCriterion()) {
//
//            // reset scores and counters
//            fill(destroy_scores_.begin(), destroy_scores_.end(), 0.0);
//            fill(repair_scores_.begin(), repair_scores_.end(), 0.0);
//            fill(destroy_counters_.begin(), destroy_counters_.end(), 0);
//            fill(repair_counters_.begin(), repair_counters_.end(), 0);
//
//            for (int it = 0; it < segment_iterations_; ++it) {
//                ++iterations_;
//                cout << "iter" << flush;
//
//                // sample move from neighborhood
//                sampleMove();
//
////                candidate_solution_.computeCost(*costs_);
//                candidate_solution_cost_ = computeWeightedCost(candidate_solution_);
//
//                delta_ = candidate_solution_cost_ - cost_memory_[memory_lookup_];
//                threesold_delta_ = -temperature_ * log(Random::continuous());
//
//                // print costs
////                cout << "real cost = "  << candidate_solution_.cost_ << " real best = " << real_best_solution.cost_<< endl;
////                cout << "fake cost " << candidate_solution_cost_ << " best fake = " << best_solution_cost_ << endl;
////                for (auto &cost : *costs_)
////                    cout << " - " << cost->name_ << " = " << candidate_solution_.cost_map_.back()[cost->id_] << endl;
////                cout << endl;
//
//                // update the scores
//                updateScores();
//
//
//                // check if solution is new best wrt real costs
//                if (candidate_solution_.cost_ < real_best_solution.cost_) {
//                    candidate_solution_.extractBestSolution(real_best_solution);
//                }
//
//
//                // accept/reject
//                if (acceptCandidateSolution()) {
//                    if (configParameter:print_to_screen) {
//                        cout << "\r" << " - Accepted. Cost = " << fixed << candidate_solution_.cost_ << flush;
//                    }
//
//
//                    // update
//                    current_solution_ = candidate_solution_;
//                    cost_memory_[memory_lookup_] = candidate_solution_cost_;
//                    for (auto &cost: *costs_)
//                        cost_memory_components_[memory_lookup_][cost->id_] = candidate_solution_.cost_map_.back()[cost->id_];
//                    memory_lookup_ = (memory_lookup_ + 1) & length_memory_;
//
//                    // check if it's best solution
//                    if (candidate_solution_cost_ < best_solution_cost_) {
//                        candidate_solution_.extractBestSolution(best_solution_);
//                        best_solution_cost_ = candidate_solution_cost_;
//                        if (notify_clusterisation_) {
//                            clusterisation_->newBestSolutionFound(&best_solution_, day_);
//                        }
//                    }
//                    // check if it's best feasible solution
//                    if (candidate_solution_cost_ < best_feasible_solution_cost_ and
//                        candidate_solution_.checkCapacityFeasibility(day_) and
//                        candidate_solution_.checkTimeFeasibility(day_)) {
//                        candidate_solution_.extractBestSolution(best_feasible_solution_);
//                        best_feasible_solution_cost_ = candidate_solution_cost_;
//                    }
//
//                }
//
//                // statistics
////                cost_evolution_.push_back(current_solution_.cost_);
////                for (auto &cost : *costs_)
////                    cost_components_evolution_[cost->id_].push_back(current_solution_.cost_map_.back()[cost->id_]);
//
//
//                // temperature
//                updateTemperature();
////                temperature_evolution_.push_back(temperature_);
//            }
//
//            // update weights
//            updateWeights();
//
//            // adjust costs
//            adjustCosts();
//
//            // if we are using districting cost reorder the routes
//            if (cost_districting and not single_day_modality_)
//                reorder_operator_.reorderRoutes(&current_solution_);
//
//            if (not configParameter:print_to_screen) {
//                cout << "\r" << toFormatString(100 * double(iterations_) / true_max_iterations_, 2) << "%";
//                cout << "   RealBest = " << format(toFormatString(real_best_solution.cost_, 2), 12);
////                cout << "   LNSBest = "<< format(toFormatString(best_solution_.cost_,2),12);
//                cout << "   Current = " << format(toFormatString(current_solution_.cost_, 2), 12);
////                cout << " Duration = " << format(toFormatString(current_solution_.computeAverageTimeGap(), 2), 4);
////                cout << " Profit = " << format(toFormatString(current_solution_.computeAverageProfitGap(), 2), 4);
////                cout << " CostProfit = " << format(toFormatString(current_solution_.cost_map_.back()[configParameter:cost_balance_profit_id], 2), 18);
////                cout << " Fleet = " << format(toFormatString(current_solution_.cost_map_.back()[configParameter:cost_fleet_size_id] /1000000, 2), 18);
//                if (single_day_modality_)
//                    cout << " Unserved = " << format(toFormatString(current_solution_.routes_length_[day_].back(), 2), 18);
//                cout << std::flush;
//
//            }
////            cout << endl;
//
//        }
//
//    }
//
//    finalizeRun();
}

void LargeNeighborhoodSearch::finalizeRun() {
//    // rebuild best_solutin
//    best_solution_ = Solution(best_solution_.data_, &best_solution_.successors_);
//    best_solution_.computeCost(*costs_);
//
//    best_feasible_solution_ = Solution(best_feasible_solution_.data_, &best_feasible_solution_.successors_);
//    best_feasible_solution_.computeCost(*costs_);
//
//    real_best_solution = Solution(real_best_solution.data_, &real_best_solution.successors_);
//    real_best_solution.computeCost(*costs_);
//
//
//    // if we are using districting cost reorder the routes
//    if (cost_districting and not single_day_modality_) {
//        reorder_operator_.reorderRoutes(&best_solution_);
//        reorder_operator_.reorderRoutes(&best_feasible_solution_);
//        reorder_operator_.reorderRoutes(&real_best_solution);
//    }
//
//    // end measuring time
//    current_time_ = clock();
//    time_ = double(current_time_ - start_time_) / CLOCKS_PER_SEC;
//    best_solution_.time_ = time_;
//    best_feasible_solution_.time_ = time_;
//    real_best_solution.time_ = time_;
}



/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::printInfo() {
//    cout << endl;
//    cout << "Iterations :        " << iterations_ << endl;
//    cout << "Time :              " << time_ << endl;
//    cout << "Idle Iterations :   " << idle_iterations_ << endl;
//    cout << "Initial Cost :      " << initial_solution_.cost_ << endl;
//    cout << "Best Cost :         " << best_solution_.cost_ << endl;
//    cout << "Cost BreakDown" << endl;
//    for (auto &cost : *costs_) {
//        cout << "    " << cost->name_ << " = " << cost->computeCost(best_solution_) << endl;
//    }
//    assert(best_solution_.checkCosts(*costs_));
}



/** ------------------------------------------------------------------------------------------------ */

void LargeNeighborhoodSearch::printInfoOnFile(string file_name) {
//    // write solutin
//    if (configParameter:print_solutions)
//        best_solution_.write(output_folder_ + "/solution-best-" + best_solution_.data_.name_ + ".txt", costs_);
//
//    // Save stuff on file
//    ofstream out_file;
//    out_file.open(file_name);
//
//    out_file << "time " << time_ << endl << endl;
//    out_file << "iterations " << iterations_ << endl << endl;
//    out_file << "cost evolution" << endl;
//    for (auto &el : cost_evolution_)
//        out_file << el << " ";
//    out_file << endl << endl;
//
//    out_file << "cost_legend" << endl;
//    for (int i = 0; i < costs_->size(); ++i)
//        out_file << i << " = " << costs_->at(i)->name_ << endl;
//    out_file << endl << endl;
//
//    out_file << "cost components evolution" << endl;
//    for (int i = 0; i < costs_->size(); ++i) {
//        for (auto &el : cost_components_evolution_[costs_->at(i)->id_])
//            out_file << el << " ";
//        out_file << endl;
//    }
//    out_file << endl << endl;
//
//    out_file << "destroy_legend" << endl;
//    for (int i = 0; i < destroy_methods_.size(); ++i) {
//        if (destroy_methods_[i]->name_ == "relatedRemovalOperator")
//            out_file << i << " = related-"
//                     << static_cast<RelatedRemovalOperator *>(destroy_methods_[i])->relatedness_measure_function_;
//        else
//            out_file << i << " = " << destroy_methods_[i]->name_;
//        if (not destroy_methods_[i]->by_day_)
//            out_file << "-h";
//        out_file << endl;
//    }
//    out_file << endl << endl;
//
//    out_file << "repair_legend" << endl;
//    for (int i = 0; i < repair_methods_.size(); ++i) {
//        if (repair_methods_[i]->name_ == "regretInsertionOperator")
//            out_file << i << " = regret-" << static_cast<RegretInsertionOperator *>(repair_methods_[i])->regret_
//                     << endl;
//        else
//            out_file << i << " = " << repair_methods_[i]->name_ << endl;
//        if (not repair_methods_[i]->by_day_)
//            out_file << "-h";
//    }
//    out_file << endl << endl;
//
//    out_file << "weights_destroy" << endl;
//    for (auto&vec : destroy_weights_evolution_) {
//        for (auto &el : vec)
//            out_file << el << " ";
//        out_file << endl;
//    }
//    out_file << endl << endl;
//
//    out_file << "weights_repair" << endl;
//    for (auto&vec : repair_weights_evolution_) {
//        for (auto &el : vec)
//            out_file << el << " ";
//        out_file << endl;
//    }
//    out_file << endl << endl;
//
////    out_file << "temperature evolution" << endl;
////    for (auto &el : temperature_evolution_)
////        out_file << el << " ";
////    out_file << endl << endl;
//
//    out_file.close();

};
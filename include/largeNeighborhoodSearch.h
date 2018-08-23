#pragma once

#include "solutionOperator.h"
#include "metaheuristic.h"
#include "cost.h"
#include "random.h"
#include "solution.h"
#include <string>
#include <ctime>
#include <unordered_set>
#include <unordered_map>


class LargeNeighborhoodSearch : public Metaheuristic {
    /** CONSTRUCTORS */
public:
    LargeNeighborhoodSearch(Solution &initial_solution, std::vector<Cost *> *costs, std::vector<SolutionOperator *> &destroy_methods, std::vector<SolutionOperator *> &repair_methods, std::string output_folder = "");

    /** METHODS */
public:
    /** run LNS */
    virtual void run();

    /** adjust the weigths at the start of segment */
    virtual void updateWeights();

    /** adjust the scores at every iterations*/
    virtual void updateScores();

    /** cool down */
    virtual void updateTemperature();

    /** print information */
    virtual void printInfo();

    /** print information */
    virtual void printInfoOnFile(std::string file_name = "");


    /** select the destroy/repairmethod to use */
    void selectDestroyMethod();
    void selectRepairMethod();

    /** finalize the run by creating best solutions, reordering routes and saving time*/
    void finalizeRun();

    /** move the solution with destroy repair */
    virtual void sampleMove();

    /** reheat the temperature */
    virtual void reheatTemperature();

    /** reset termperature, max_iterations and counters */
    virtual void reset();

    /** compute the initial temperature */
    virtual void computeInitialTemperature();

    /** stop condition */
    virtual bool stoppingCriterion();

    /** stop condition */
    virtual bool acceptCandidateSolution();


    /** identify the cost components that need manipulation of the solution before/after the insertion search, non zero costs and set the weights */
    void setUpCosts();

    /** adjust the cost based on the iteration */
    void adjustCosts();

    /** compute weighted cost of a solution using the weights*/
    double computeWeightedCost(Solution &solution);




    /** VARIABLES */
public:
    /** the initial and current solution */
    Solution &initial_solution_;
    Solution best_solution_, current_solution_, best_feasible_solution_;

    /** the candidate solution obtained through destroy/repair the current solution */
    Solution candidate_solution_;

    /** [cost.id_] = weight for the cost importance.
     * Note. I use map cause we might not use all the costs abut we still use cost.id to access cost_weights_ */
    std::unordered_map<int, double> cost_weights_;

    /** costs computed using weights of lns */
    double initial_solution_cost_, best_solution_cost_, current_solution_cost_, best_feasible_solution_cost_, candidate_solution_cost_;


    /** cost memory */
    std::vector<double> cost_memory_;

    /** cost components memory. Needed to update cost_memoery if we are increasing some cost components */
    std::vector<std::unordered_map<int, double>> cost_memory_components_;

    /** index to access memory cost */
    int memory_lookup_;

    /** the set of visited solutions */
    std::unordered_set<size_t> visited_solution_hashes_;

    /** the weigths for destroy-repair methods */
    std::vector<double> destroy_weights_, repair_weights_;

    /** the scores for destroy-repair methods */
    std::vector<double> destroy_scores_, repair_scores_;

    /** the counter for destroy-repair methods */
    std::vector<int> destroy_counters_, repair_counters_;

    /** the probabilities for destroy-repair methods */
    std::vector<double> destroy_cumulative_probabilities_, repair_cumulative_probabilities_;


    /** the methods to destroy/repair solution */
    std::vector<SolutionOperator*> &destroy_methods_, &repair_methods_;

    /** the chosen methods */
    int selected_destroy_, selected_repair_;

    /** the delta in the cost (candidate - current) */
    double delta_;

    /** the trheshold for the delta of the move */
    double threesold_delta_;


    /** count how many times we reheated */
    int reheat_counter_;

    /** current temperature */
    double temperature_;

    /** whether to collect the best solutions or not */
    bool notify_clusterisation_;


    /** LNS PARAMETERS  ---------------------------------------------- */
    /** the late acceptance memeory length */
    int length_memory_;

    /** the number of iterations in a segment */
    int segment_iterations_;

    /** the reactivity in adjustment of weights */
    double reactivity_;

    /** the scores updates */
    double sigma_1_, sigma_2_, sigma_3_;

    /** maximum number of total iterations */
    int max_iterations_;

    /** SA PARAMETERS  ---------------------------------------------- */
    /** initial temperature */
    double initial_temperature_;

    /** end temperature */
    double end_temperature_;

    /** the cooling down rate */
    double cooling_rate_;

    /** max time we can reheat */
    int max_reheats_;

    /** used for computing initial solution. Probability of accepting a given deterioration */
    double initial_probability_, accepted_deterioration_;

    /** to measure time */
    clock_t start_time_;
    clock_t current_time_;

    /** -------------------- STATISTICS FIELD -------------------- */
    /** max iteration computer after choosing the temperature */
    int true_max_iterations_;

    /** evolution of methods weights */
    std::vector<std::vector<double>> destroy_weights_evolution_, repair_weights_evolution_;

    /** methods chosen at each iteration */
    std::vector<int> destroy_type_, repair_type_;

    /** temperature evolution*/
    std::vector<double> temperature_evolution_;

    /** cost evolution */
    std::vector<double> cost_evolution_;


    /** [cost_id] = cost evolution */
    std::unordered_map<int, std::vector<double>> cost_components_evolution_;





};
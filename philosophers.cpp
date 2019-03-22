#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>
 
std::mutex g_lockprint;
constexpr  int no_of_philosophers = 5;
 
struct fork
{
   std::mutex mutex;
};
 
struct table
{
   std::atomic<bool>                    ready{ false };
   std::array<fork, no_of_philosophers> forks;
};
 
struct philosopher
{
private:
   std::string const name;
   table const &     dinnertable;
   fork&             left_fork;
   fork&             right_fork;
   std::thread       lifethread;
   std::mt19937      rng{ std::random_device{}() };
public:
   philosopher(std::string n, table const & t, fork & l, fork & r) :
      name(n), dinnertable(t), left_fork(l), right_fork(r), lifethread(&philosopher::dine, this)
   {
   }
 
   ~philosopher()
   {
      lifethread.join();
   }
 
   void dine()
   {
      while (!dinnertable.ready);
 
      do
      {
         think();
         eat();
      } while (dinnertable.ready);
   }
 
   void print(std::string text)
   {
      std::lock_guard<std::mutex> cout_lock(g_lockprint);
      std::cout
         << std::left << std::setw(10) << std::setfill(' ')
         << name << text << std::endl;
   }
 
   void eat()
   {
      std::lock(left_fork.mutex, right_fork.mutex);
 
      std::lock_guard<std::mutex> left_lock(left_fork.mutex,   std::adopt_lock);
      std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);
 
      print(" started eating.");
 
      static thread_local std::uniform_int_distribution<> dist(1, 6);
      std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));
 
      print(" finished eating.");
   }
 
   void think()
   {
     print(" is thinking ");

     static thread_local std::uniform_int_distribution<> wait(1, 6);
     std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));
   }
};
 
void dine()
{
   std::this_thread::sleep_for(std::chrono::seconds(1));
   std::cout << "Dinner started!" << std::endl;
 
   {
      table _table;
      std::array<philosopher, no_of_philosophers> philosophers
      {
         {
            { "Aristotle", _table, _table.forks[0], _table.forks[1] },
            { "Platon",    _table, _table.forks[1], _table.forks[2] },
            { "Descartes", _table, _table.forks[2], _table.forks[3] },
            { "Kant",      _table, _table.forks[3], _table.forks[4] },
            { "Nietzsche", _table, _table.forks[4], _table.forks[0] },
         }
      };
 
      _table.ready = true;
      std::this_thread::sleep_for(std::chrono::seconds(5));
      _table.ready = false;
   }
 
   std::cout << "Dinner done!" << std::endl;
}
 
int main()
{   
   dine();
   system("pause");
 
   return 0;
}
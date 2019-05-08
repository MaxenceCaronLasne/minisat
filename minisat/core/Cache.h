#ifndef Minisat_Cache_h
#define Minisat_Cache_h

#include <deque>
#include <set>
#include <algorithm>
#include <iostream>
#include "SolverTypes.h"

namespace Minisat {
        class Cache {
        private:
                std::deque<Var> _queue;
                std::set<Var> _set;
                size_t _depth;
                int _width;
                size_t _cache_hit;
                size_t _total;

                void add_single(Var v)
                {
                        if (_queue.size() >= _depth - 1) {
                                _set.erase(_queue.back());
                                _queue.pop_back();
                        }
                        _queue.push_front(v);
                        _set.insert(v);
                }

                void add(Var v)
                {
                        for (Var i = v - _width; i < v + _width + 1; ++i)
                                add_single(i);
                }

                bool is_hit(Var v)
                {
                        auto elm = _set.find(v);
                        return elm != std::end(_set);
                }

        public:
                Cache(size_t depth, size_t width):
                        _queue(std::deque<Var>()),
                        _set(std::set<Var>()),
                        _depth(depth),
                        _width(width),
                        _cache_hit(0) {}

                ~Cache() {}

                void set_depth(size_t size) { _depth = size; }

                void submit(Var v)
                {
                        _total++;

                        if (is_hit(v)) {
                                _cache_hit++;
                                return;
                        }

                        add(v);
                }

                void get_results()
                {
                        std::cout << "| " << _depth << " | " << _width << " | "
                                  << (float)_cache_hit / (float)_total
                                  << " |" << '\n';
                }
        };

        extern Cache *cache_sim;
}


#endif // !Minisat_Cache_h

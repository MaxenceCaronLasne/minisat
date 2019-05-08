#ifndef Minisat_Cache_h
#define Minisat_Cache_h

#include <vector>
#include <algorithm>
#include <iostream>
#include "SolverTypes.h"

namespace Minisat {
        class Cache {
        private:
                std::vector<Var> _queue;
                size_t _depth;
                int _width;
                size_t _cache_hit;
                size_t _total;

                void add_single(Var v)
                {
                        if (_queue.size() >= _depth - 1)
                                _queue.pop_back();
                        _queue.insert(std::begin(_queue), v);
                }

                void add(Var v)
                {
                        for (Var i = v - _width; i < v + _width + 1; ++i)
                                add_single(i);
                }

                bool is_hit(Var v)
                {
                        auto elm = std::find(std::begin(_queue),
                                             std::end(_queue), v);
                        return elm != std::end(_queue);
                }

        public:
                Cache(size_t depth, size_t width):
                        _queue(std::vector<Var>()),
                        _depth(depth),
                        _width(width),
                        _cache_hit(0) {}

                ~Cache() {}

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
                        std::cout << "| " << _depth << " | " << _width << " | " << (float)_cache_hit / (float)_total << " |" << '\n';
                }
        };

        extern Cache *cache_sim;
}


#endif // !Minisat_Cache_h

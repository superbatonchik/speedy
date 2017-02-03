#pragma once
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

#define secs(_time_) (_time_ % SECS_PER_MIN)  
#define mins(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define hours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define days(_time_) ( _time_ / SECS_PER_DAY)
//
// Created by Jason Zheng on 2020/3/24.
//

#ifndef SANDRA_STR_H
#define SANDRA_STR_H

#include <string.h>

#define sdr_str_to_n_base_long_long(str, num, n) ({ \
   char *m_stnbll_end; \
   size_t m_stnbll_slen = strlen(str); \
   num = strtoll(str, &m_stnbll_end, n); \
   ((m_stnbll_end - (str)) == m_stnbll_slen); \
})

#define sdr_str_to_n_base_long(str, num, n) ({ \
   char *m_stnbl_end; \
   size_t m_stnbl_slen = strlen(str); \
   num = strtol(str, &m_stnbl_end, n); \
   ((m_stnbl_end - (str)) == m_stnbl_slen); \
})

#define sdr_str_to_long_long(str, num) \
    sdr_str_to_n_base_long_long(str, num, 10)

#define sdr_str_to_long(str, num) \
    sdr_str_to_n_base_long(str, num, 10)

#define sdr_str_to_long_double(str, num) ({ \
   char *m_stld_end; \
   size_t m_stld_slen = strlen(str); \
   num = strtold(str, &m_stld_end); \
   ((m_stld_end - (str)) == m_stld_slen); \
})

#define sdr_str_to_double(str, num) ({ \
   char *m_std_end; \
   size_t m_std_slen = strlen(str); \
   num = strtod(str, &m_std_end); \
   ((m_std_end - (str)) == m_std_slen); \
})

#endif //SANDRA_STR_H

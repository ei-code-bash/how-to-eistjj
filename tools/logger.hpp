/*
 * @Author: ei_code_bash && 3080152159@qq.com
 * @Date: 2025-11-08 14:32:04
 * @LastEditors: ei_code_bash && 3080152159@qq.com
 * @LastEditTime: 2025-11-08 20:35:32
 * @FilePath: /how-to-eistjj/tools/logger.hpp
 * @Description: 我永远喜欢雪之下雪乃
 * 
 * Copyright (c) 2025 by ei_code_bash, All Rights Reserved. 
 */
#ifndef __LOGGER_H_
#define __LOGGER_H_
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <string>
#include <sys/stat.h>
#include <stdarg.h>

#define FILE_LOGGER_VERSION    "V1.0,我永远喜欢雪之下雪乃"
#define FILE_LOGGER_VERSION_INFO "okkk, it just give a logger"

#define LOG_FILE_LINE( x )                                           \
*(( x ).get_ostream() ) << __FILE__ << "" << __LINE__ <<std::endl;\
( x ).get_ostream()->flush();//刷新缓存

#define LOG_FILE_LINE_AB( a, b )                                        \
*(( a ).get_ostream(b) ) << __FILE__ << "" << __LINE__ <<std::endl;\

#define LOG_FUNCTION_FINE( x )                                          \
*(( x ).get_ostream() ) <<__FILE__ << "" << __LINE__ <<std::endl;\
( x ).get_ostream()->flush();

#define LOG_FUNCTION_FINE_AB( a, b )                                    \
*(( a ).get_ostream( b )) << __FILE__ << "" << __LINE__ <<std::endl;\ 

namespace ne_tools
{
    class File_logger
    {
        public:
        File_logger()
        {
            
        };
        File_logger(std::string _dir_name_ = std::string(""))
        {
            set_log_sir(_dir_name_);
            m_map_index_os_.insert( std::pair<std::string, std::ostream*> ("screen", &std::cout));
        }

        ~File_logger()
        {
            release();
        }
        private:
        std::map<std::string , std::ostream* >m_map_index_os_;
        char temp_buffer_[10000];
        std::string fd_path_;
        std::string fd_saved_path = std::string ("");
        
        void release()
        {
          for(auto it = m_map_index_os_.begin(); it != m_map_index_os_.end(); it++)
          {
             it->second->flush();
             (it ->second) = NULL;
             delete it ->second;
            
          }  
        };

        void set_log_sir(std::string _dir_name_)
        {
            release();
            fd_saved_path = _dir_name_;
            mkdir( fd_saved_path.c_str(), 0775);
        };

        std::string version()
        {
          std::stringstream ss;
          ss << "====== This is version of File_logger ====="<< std::endl;
          ss << "Version :" <<FILE_LOGGER_VERSION <<std::endl;
          ss << "Version info "<< FILE_LOGGER_VERSION_INFO <<std::endl;
          ss << "Compile data " << __DATE__ << __TIME__<< std::endl;
          ss << "==== End ====="<<std::endl;
          return std::string(ss.str());
        };

        void init(std::string _file_name, std::string prefix_name = std::string( "log" ),int mode = std::ios::out )
        {
            std::ofstream* ofs = new std::ofstream();
            sprintf( temp_buffer_, "%s/%s_%s", fd_saved_path.c_str(), prefix_name.c_str(),_file_name.c_str());
            ofs ->open(temp_buffer_, std::ios::out);
            if( ofs ->is_open())
            {
                std::cout <<"Open"<<_file_name <<"successful." <<std::endl;
                m_map_index_os_.insert( std::pair<std::string, std::ostream*> (prefix_name, ofs));
            }
            else
            {
                std::cout << "fail to open" <<_file_name << std::endl;
                m_map_index_os_.insert(std::pair<std::string,std::ostream*>(prefix_name, &std::cout));
            
            }
        };

            std::ostream* get_ostream( std::string prefix_name = std::string ("log"))
            {
                auto it = m_map_index_os_.find(prefix_name);
                
                if( it != m_map_index_os_.end())
                {
                    return (it ->second);
                }
                else
                {
                    init("", prefix_name);
                    return get_ostream (prefix_name);
                }
            };
            int printf(const char * fmt, ...)
            {
                va_list ap;
                char* result = 0;
                va_start (ap, fmt);
                if(vasprintf (&result, fmt, ap) ==0)
                {
                    return 0;
                }
                fd_path_ = std::string (result);
                * ( get_ostream() ) << fd_path_ ;
                ( get_ostream ( "log" ) )->flush();
                return fd_path_.length();
            };
            
            
            
            
        

        



        
        
    };
}




#endif
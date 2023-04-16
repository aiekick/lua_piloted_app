#include "FileDataProvider.h"

#include <string>

FileDataProvider::FileDataProvider()
{

}

FileDataProvider::~FileDataProvider()
{

}

void FileDataProvider::init()
{

}

void FileDataProvider::unit()
{

}

bool FileDataProvider::buildNode(const sol::table& vTable)
{
    for (auto& item : vTable)
    {
        auto& key = item.first;
        auto& value = item.second;

        const auto& key_type = key.get_type();
        const auto& value_type = value.get_type();

        if (key_type == sol::type::string &&
            value_type == sol::type::string)
        {
            auto key_as_string = key.as<std::string>();
            if (key_as_string == "file")
            {
                auto value_as_string = value.as<std::string>();
                setFilepathName(value_as_string);
            }
        }
        else if (key_type == sol::type::string &&
            value_type == sol::type::table)
        {
            auto key_as_string = key.as<std::string>();
            if (key_as_string == "format")
            {
                sol::table format_table = value.as<sol::table>();
                if (!format_table.empty())
                {
                    for (auto& channels : format_table)
                    {
                        auto& channel_key = channels.first;
                        auto& channel_value = channels.second;

                        const auto& channel_key_type = channel_key.get_type();
                        const auto& channel_value_type = channel_value.get_type();

                        if (channel_key_type == sol::type::string &&
                            channel_value_type == sol::type::table)
                        {
                            auto channel_key_as_string = channel_key.as<std::string>();
                            sol::table channel_value_as_table = channel_value.as<sol::table>();
                            
                            std::string name;
                            std::string type;
                            uint32_t count;
                            double default_value;

                            for (auto& params : channel_value_as_table)
                            {
                                auto& param_key = params.first;
                                auto& param_value = params.second;

                                const auto& param_key_type = param_key.get_type();
                                const auto& param_value_type = param_value.get_type();

                                if (param_key_type == sol::type::string)
                                {
                                    auto param_key_as_string = param_key.as<std::string>();
                                    if (param_key_as_string == "name")
                                    {
                                        if (param_value_type == sol::type::string)
                                        {
                                            name = param_value.as<std::string>();
                                        }
                                    }
                                    else if (param_key_as_string == "type")
                                    {
                                        if (param_value_type == sol::type::string)
                                        {
                                            type = param_value.as<std::string>();
                                        }
                                    }
                                    else if (param_key_as_string == "count")
                                    {
                                        if (param_value_type == sol::type::number)
                                        {
                                            count = (uint32_t)param_value.as<double>();
                                        }
                                    }
                                    else if (param_key_as_string == "default_value")
                                    {
                                        if (param_value_type == sol::type::number)
                                        {
                                            default_value = param_value.as<double>();
                                        }
                                    }
                                }
                            }

                            // create channel
                            printf("channel name : %s\n", name.c_str());
                            printf("channel type : %s\n", type.c_str());
                            printf("channel count : %u\n", count);
                            printf("channel def_value : %f\n", default_value);

                            setValue(name, default_value);
                        }
                    }
                }
            }
        }
    }

	return true;
}

void FileDataProvider::setFilepathName(const std::string& vFilePathName)
{
	m_FilePathName = vFilePathName;
}

void FileDataProvider::start_read_op(const double& vPeriod)
{

}

void FileDataProvider::stop_read_op()
{

}

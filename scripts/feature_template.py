"""
@file:   feature_template.py
@author: Zeyu Yang (zeyuuyang42@gmail.com)
@date:   June 28, 2023
@version 0.1
@Description:
    This script generate the template .h and .cpp file of feature algorithm
    in the format defined in FeatureExtractor vitual class

@copyright Copyright (c) 2023
"""
import argparse

def create_head_template(feature_name):
    head_template = []

    head_template.append(f'#ifndef {feature_name.upper()}_H')
    head_template.append(f'#define {feature_name.upper()}_H')
    head_template.append('')
    head_template.append('#include "configs.h"')
    head_template.append('#include "featureextractor.h"')
    head_template.append('')
    head_template.append('namespace zerr {')
    head_template.append('namespace feature{')
    head_template.append('/**')
    head_template.append(f'* {feature_name} algorithm')
    head_template.append('*/')
    head_template.append(f'class {feature_name} : public FeatureExtractor {{ ')
    head_template.append('public:')
    head_template.append('    static const std::string name;')
    head_template.append('    static const std::string category;')
    head_template.append('    static const std::string description;')
    head_template.append('')
    head_template.append('    std::string get_name(){return name;}')
    head_template.append('    std::string get_category(){return category;}')
    head_template.append('    std::string get_description(){return description;}')
    head_template.append('')
    head_template.append('    void initialize(t_systemConfigs sys_cfg);')
    head_template.append('    void extract();')
    head_template.append('    void reset();')
    head_template.append('    void fetch(t_featureInputs in);')
    head_template.append('    t_featureValue send();')
    head_template.append('')
    head_template.append('private:')
    head_template.append('')
    head_template.append('    void _reset_param();')
    head_template.append('};')
    head_template.append('')
    head_template.append('} //namespace feature')
    head_template.append('} //namespace zerr')
    head_template.append(f'#endif // {feature_name.upper()}_H')

    # add break
    for i in range(len(head_template)):
        head_template[i] = head_template[i] + '\n'

    return head_template


def create_src_template(feature_name):
    src_template = []

    src_template.append('#include "utils.h"')
    src_template.append(f'#include "{feature_name.lower()}.h"')
    src_template.append('')
    src_template.append('using namespace zerr;')
    src_template.append('using namespace feature;')
    src_template.append('')
    src_template.append(f'const std::string {feature_name}::name        = "";')
    src_template.append(f'const std::string {feature_name}::category    = "";')
    src_template.append(f'const std::string {feature_name}::description = "";')
    src_template.append('')
    src_template.append(f'void {feature_name}::initialize(t_systemConfigs sys_cfg){{')
    src_template.append('    system_configs = sys_cfg;')
    src_template.append('')
    src_template.append('    _reset_param();')
    src_template.append('    if (is_initialized()==false){')
    src_template.append('        set_initialize_statue(true);')
    src_template.append('    }')
    src_template.append('}')
    src_template.append('')
    src_template.append(f'void {feature_name}::extract(){{')
    src_template.append('')
    src_template.append('}')
    src_template.append('')
    src_template.append(f'void {feature_name}::reset(){{')
    src_template.append('')
    src_template.append('}')
    src_template.append('')
    src_template.append(f'void {feature_name}::fetch(t_featureInputs in){{')
    src_template.append('')
    src_template.append('}')
    src_template.append('')
    src_template.append(f't_featureValue {feature_name}::send(){{')
    src_template.append('    return y;')
    src_template.append('}')
    src_template.append('')
    src_template.append(f'void {feature_name}::_reset_param(){{')
    src_template.append('')
    src_template.append('}')

    # add break
    for i in range(len(src_template)):
        src_template[i] = src_template[i] + '\n'

    return src_template

def main(name):
    feature_name = name
    
    head_name = f"{feature_name.lower()}.h"
    src_name  = f"{feature_name.lower()}.cpp"

    head_template = create_head_template(feature_name)
    src_template  = create_src_template(feature_name)

    content = feature_name + "\n";

    with open(head_name, 'w') as f:
        f.writelines(head_template)

    with open(src_name, 'w') as f:
        f.writelines(src_template)


if __name__ == '__main__':

    # Parse command line input
    parser = argparse.ArgumentParser(description='Feature Template Generator')
    parser.add_argument('-n', '--name', help='name of the feature', required=True)

    args = parser.parse_args()

    main(args.name)
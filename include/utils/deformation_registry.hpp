#pragma once

#include "deformation/deformation_factory.hpp"

#define REGISTER_DEFORMATION(CLASS, ID) \
    namespace { \
        struct CLASS##Registrar { \
            CLASS##Registrar() { \
                DeformationFactory::registerDeformation(ID, std::make_shared<CLASS>()); \
            } \
        }; \
        static CLASS##Registrar CLASS##registrar_instance; \
    }

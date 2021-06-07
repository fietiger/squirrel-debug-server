
#ifndef SAMPLE_APP_SWAGGER_COMPONENT_H
#define SAMPLE_APP_SWAGGER_COMPONENT_H

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

namespace qdb {
/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */
class SwaggerComponent {
 public:
  /**
   *  General API docs info
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)
  ([] {
    oatpp::swagger::DocumentInfo::Builder builder;

    builder
            .setTitle("User entity service")
            .setDescription("CRUD API Example project with swagger docs")
            .setVersion("1.0")
            .setContactName("Ivan Ovsyanochka")
            .setContactUrl("https://oatpp.io/")

            .setLicenseName("Apache License, Version 2.0")
            .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")

            .addServer("http://localhost:8000", "server on localhost");

    return builder.build();
  }());


  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)
  ([] {
    return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
  }());
};
}// namespace qdb

#endif /* SAMPLE_APP_SWAGGER_COMPONENT_H */
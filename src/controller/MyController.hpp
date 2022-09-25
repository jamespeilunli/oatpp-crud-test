#ifndef MyController_hpp
#define MyController_hpp

#include <string>
#include <iostream>
#include <fstream>

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  ENDPOINT("GET", "/", root) {
    auto dto = MyDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }
  
  // TODO Insert Your endpoints here !!!

  // https://github.com/oatpp/oatpp/issues/20
  ENDPOINT("GET", "isEven/*", getUserWithQueryParams,
           REQUEST(std::shared_ptr<IncomingRequest>, request) // Map request object to endpoint method
           ) {

    /* get url 'tail' - everything that comes after '*' */
    String tail = request->getPathTail(); // everything that goes after '*'

    /* check tail for null */
    OATPP_ASSERT_HTTP(tail, Status::CODE_400, "null query-params");

    /* parse query params from tail */
    auto queryParams = oatpp::network::Url::Parser::parseQueryParams(tail);

    /* get your param by name */
    std::string input = queryParams.get("input");

    // act upon the input
    std::string output = stoi(input) % 2 == 0 ? "true" : "false";

    /* return result */
    return createResponse(Status::CODE_200, "output=" + output);

  }

    ENDPOINT("GET", "/create", getUsers,
           QUERIES(QueryParams, queryParams)) {

           std::ofstream data_file;
           data_file.open ("data.json");
           data_file << "{";
           bool first = true;
           for (auto& param : queryParams.getAll()) {
               int key = std::stoi(param.first.std_str());
               int value = std::stoi(param.second.std_str());

               if (first) {
                   data_file << key << ":" << value;
                   first = false;
               } else {
                   data_file << "," << key << ":" << value;
               }
           }
           data_file << "}";
           data_file.close();

        return createResponse(Status::CODE_418, "Thirsty Vitus");
    }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */

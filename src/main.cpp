#include <Rcpp.h>
using namespace Rcpp;

#include "easywsclient.hpp"
#include "ws.h"

std::mutex mtx;
std::condition_variable cv;
std::string msg;
bool ready = false;

void handle_message(const std::string & message) {
  std::unique_lock<std::mutex> lock(mtx);
  lock.unlock();
  msg = message;
  ready = true;
  lock.lock();
  cv.notify_one();
}

//' Connect to Chrome Remote
//'
//' @param url ws URL
//' @export
// [[Rcpp::export]]
SEXP chrome_connect(std::string url) {

  chromeWsPtr x = new(chromeWs);
  x->ws = easywsclient::WebSocket::from_url(url);
  x->response = "Hi";
  x->ready = false;

  assert(x->ws);

  return(XPtrWs(x));

};

//' Instrument Chrome
//'
//' @export
// [[Rcpp::export]]
std::string instrument(SEXP ws_ptr, std::string cmd) {

  std::cout << ((chromeWsPtr)ws_ptr)->response << " " << cmd << std::endl;

  return("");

}
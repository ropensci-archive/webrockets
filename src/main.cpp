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
  x->response = std::string("Hi");
  x->ready = false;

  assert(x->ws);

  while(x->ws->getReadyState() != easywsclient::WebSocket::OPEN) {
    Rcpp::checkUserInterrupt();
  }

  return(XPtrWs(x));

};

//' Instrument Chrome
//'
//' @export
// [[Rcpp::export]]
std::string instrument(SEXP ws_ptr, std::string cmd) {

  //int i=0;

  chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));

  ready = false;

  wsp->ws->send(cmd);

  // while (!ready) {
  //   i++;
  //   if((i % 100) == 0) Rcpp::checkUserInterrupt();
    // std::cout << "." ;
  wsp->ws->poll(-1);
  wsp->ws->dispatch(handle_message);
  // }

  // std::cout<<std::endl;

  return(msg);

}

//' Consume event
//'
//' @export
// [[Rcpp::export]]
std::string ws_poll(SEXP ws_ptr, int timeout=5) {

  chromeWsPtr wsp = ((chromeWsPtr)R_ExternalPtrAddr(ws_ptr));

  ready = false;

  while (!ready) {
    std::cout << "." ;
    wsp->ws->poll(5);
    wsp->ws->dispatch(handle_message);
  }

  std::cout<<std::endl;

  return(msg);

}

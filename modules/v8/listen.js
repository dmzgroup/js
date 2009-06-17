(function () {

function listen(event, es, ed, data) {

   if (event == V8.Debug.DebugEvent.Exception) {

      for (var ix = 0; ix < es.frameCount(); ix++) {

         DMZ.Debug.addStackTrace (
            ix,
            es.frame(ix).func().script().name(),
            es.frame(ix).sourceLine(),
            es.frame(ix).sourceColumn(),
            es.frame(ix).sourceLineText());
      }
   }
}

V8.Debug.setListener(listen);
V8.Debug.setBreakOnException();

})();

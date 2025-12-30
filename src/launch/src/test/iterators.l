# --pragma-test

# --pragma-test-list-apps
for it: (apps.begin, apps.end) {
   it.must_pass();
}

# --pragma-test-start
when start: (apps) {
     it (apps);
}

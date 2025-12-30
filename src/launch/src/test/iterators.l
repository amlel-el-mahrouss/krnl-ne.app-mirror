# --pragma-test

# --test-list-apps
for it: (apps.begin, apps.end) {
   it.print();
}

# --test-start
when start: (apps) {
     it (apps);
}

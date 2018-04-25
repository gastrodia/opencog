LINK_LIBRARIES(
  ${ATOMSPACE_LIBRARIES}
  ${COGUTIL_LIBRARY}
  commander
)

# The tests are ordered in the order they are run during make test.
ADD_CXXTEST(CommanderUTest)

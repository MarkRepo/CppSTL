all:
	bazel build ...

test:
	bazel test ...

clean:
	bazel clean

distclean:
	bazel clean --expunge	

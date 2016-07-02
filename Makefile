# Build commands
#

site: $(shell find . -path "*html" -o -path "*.md" -not -path "*/_site/*")
	bundle exec jekyll build --drafts

serve:
	bundle exec jekyll serve --drafts --watch

.PHONY: stop
stop:
	ps -A | grep -i "jekyll" | head -n 1 | cut -d" " -f1 | xargs kill -9

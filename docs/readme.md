
This composite plugin (rake task + plugin) allows you to import a Github wiki in a Jekyll site.

## install

### Create your wiki

**Note** : your wiki MUST be public and have at least One page

### Download code

- Download the [zip archive here](https://github.com/djacquel/WikiToJekyll/archive/master.zip)
- Unzip **_plugins/** and **Rakefile** in an already existing Jekyll folder

### Configure

In **_config.yml**, set you parameters for wiki import

```
##
# Parameters for WikiToJekyll
# Used by Rake:wikisub, Rake:wiki and wikiLinks generator plugin
##
wikiToJekyll:

  # your user Github Name
  user_name: djacquel

  # your repository Name
  repository_name: JekyllTest

  # set your remote name. 'origin' is the default name set
  # when you do a 'git init'
  # if you changed this name be sure to change this parameter
  deploy_remote: "origin"

  # for an organization / user, publication branch is master
  # for a project, publication branch is gh-pages
  deploy_branch: "gh-pages"

  # wiki repository url
  # if you live this blank, it will be derived from you code
  # user_name and repository_name
  #   eg : wiki_repository_url = user_name/repository_name/wiki'
  #
  # If you're importing a wiki from another code repository
  # you MUST set this url
  #
  # IMPORTANT: no git@github.com: in front
  #            You MUST use the htpps:// url or it will
  #            cause a submodule error on github
  #
  # Example : https://github.com/userName/repositoryName.wiki.git

  wiki_repository_url: # https://github.com/userName/repositoryName.wiki.git

  # wiki submodule folder
  # the underscore makes sure that this folder is ignored by Jekyll
  wiki_source: "_wiki"

  # wiki Jekyll generated pages destination folder
  wiki_dest: "wiki"

  # commit and push to Jekyll repository after wiki synchronisation
  commit_and_push: false
```

If your site is a repository site (ie: living under user.github.io/repositoryName), **don't forget** to set your **baseurl:** parameter in your **_config.yml** file.

```
baseurl: '/repositoryName' # no trailing slash !
```

### Link your Wiki to your repository as a submodule

**Once** you've configured Jekyll you can do

```
rake wikisub
```

or

```
git submodule add https://github.com/userName/repositoryName.wiki.git
git submodule init
git submodule update
```

### Building your wiki for first time

Once submodule updated, you just have to build you wiki pages.

```
rake wikibuild
```

This will :

- cause errors if you've missed some of the install steps

or

- transform downloaded wiki pages to markdown pages with yaml front matter
- launch a Jekyll build
- convert wiki links to Jekyll links

## Jekyll pages integration

By default, all wiki pages are in the **wiki** folder and all have a **menu: wiki** variable set in the front matter.

A wiki menu can be easily generated with this markup :

```
<ul class="wikiMenu">
  {% for p in site.pages %}
    {% if p.menu == "wiki" %}
    <li><a class="post-link" href="{{ p.url | prepend: site.baseurl }}">{{ p.title }}</a></li>
    {% endif %}
  {% endfor %}
</ul>
```
## Synchronize your wiki

Sometimes you'll want to sync your Jekyll site with wiki updates.
Just :

```
rake wiki
```
- sync your wiki pages
- transform them to markdown pages with yaml front matter
- launch a Jekyll build
- convert wiki links to Jekyll links
- optionally commit and push you code to you Jekyll repository

This has been made as a rake task, that's allows you to make a cron job with last command.


## Workarounds

The github wiki mardown is a little different from the default Jekyll markdown (kramdown).

```
... end of sentence
## title
- list item
- list item
```
That renders in wiki but not in Jekyll. This because of missing new lines.

In order to get your wiki page processed well, you can switch your markdown processor to redcarpet.

NAME			=	ircserv

SRC				=	main.cpp \
					Server.cpp \
					Utils.cpp \
					UtilsServer.cpp \
					CAP.cpp \
					JOIN.cpp \
					KICK.cpp \
					MODE.cpp \
					NICK.cpp \
					NOTICE.cpp \
					PART.cpp \
					PASS.cpp \
					PRIVMSG.cpp \
					QUIT.cpp \
					TOPIC.cpp \
					USER.cpp \
					WHO.cpp \
					BOT.cpp 

SRCDIR			=	./
SRCDIR_SOURCES	=	sources
SRCDIR_COMMANDS	=	commands
OBJDIR			=	./.objFiles

GITIGNORE_PATH := .gitignore
GITIGNORE_EXISTS := $(shell grep -Fxq "$(NAME)" $(GITIGNORE_PATH) && echo "yes" || echo "no")

OBJ				=	$(addprefix $(OBJDIR)/, $(addsuffix .o, $(BASE)))
BASE			=	$(basename $(notdir $(SRC)))

CC				=	c++
CXXFLAGS		=	-Wall -Wextra -Werror -g
RM				=	rm -rf
ECHO			=	echo
MKDIR			=	mkdir -p

all: $(NAME)

$(OBJDIR):
	@test -d $(OBJDIR) || $(MKDIR) $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJ)
	@$(CC) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@if [ "$(GITIGNORE_EXISTS)" = "no" ]; then \
		echo $(NAME) >> $(GITIGNORE_PATH); \
	fi
	@$(ECHO) "\033[0;32m> $(NAME) Compiled\033[0m"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@$(CC) $(CXXFLAGS) -c -o $@ $< \
	&& $(ECHO) "\033[1;30m[C++] : Compiled $(notdir $<) \
	[\033[0;32mOk\033[1;30m]\033[0m" \
	|| $(ECHO) "\033[1;30m[C++] : Compiled $(notdir $<) \
	[\033[0;7;31mError\033[0;1;30m]\033[0m"

$(OBJDIR)/%.o: $(SRCDIR_SOURCES)/%.cpp | $(OBJDIR)
	@$(CC) $(CXXFLAGS) -c -o $@ $< \
	&& $(ECHO) "\033[1;30m[C++] : Compiled $(notdir $<) \
	[\033[0;32mOk\033[1;30m]\033[0m" \
	|| $(ECHO) "\033[1;30m[C++] : Compiled $(notdir $<) \
	[\033[0;7;31mError\033[0;1;30m]\033[0m"

$(OBJDIR)/%.o: $(SRCDIR_COMMANDS)/%.cpp | $(OBJDIR)
	@$(CC) $(CXXFLAGS) -c -o $@ $< \
	&& $(ECHO) "\033[1;30m[C++] : Compiled $(notdir $<) \
	[\033[0;32mOk\033[1;30m]\033[0m" \
	|| $(ECHO) "\033[1;30m[C++] : Compiled $(notdir $<) \
	[\033[0;7;31mError\033[0;1;30m]\033[0m"

clean:
	@$(RM) $(OBJDIR)
	@$(ECHO) "\033[0;34m> object directory cleaned\033[0m"

fclean:	clean
	@$(RM) $(NAME)
	@$(ECHO) "\033[0;35m> $(NAME) removed\033[0m"

re: fclean all

.PHONY: all clean fclean re                               #

#ifndef CAPELCAPELMODULE_H_
#define CAPELCAPELMODULE_H_

#include <gongxtring.h>

using gong::Xtring;

namespace capel {

class CapelModule 
{
public:
	CapelModule() {}
    CapelModule(const Xtring &filename,
                const Xtring &begin_keyword ="<<<<<", const Xtring &end_keyword=">>>>>",
		const Xtring &begin_comment="/*", const Xtring &end_comment="*/");
    void init(const Xtring &filename,
	      const Xtring &begin_keyword ="<<<<<", const Xtring &end_keyword=">>>>>",
	      const Xtring &begin_comment="/*", const Xtring &end_comment="*/");
	virtual ~CapelModule() {}
	const Xtring &getFilename() const { return mFilename; }
    void setBeginKeyword(const Xtring bk) { mBeginKeyword = bk; }
    const Xtring &getBeginKeyword() const { return mBeginKeyword; }
    void setEndKeyword(const Xtring bk) { mEndKeyword = bk; }
    const Xtring &getEndKeyword() const { return mEndKeyword; }
    size_t write();
    size_t writeIfModified();
    Xtring get_extrusion_text(const Xtring &extrusion_name);
    int append_text_to_extrusion(const Xtring &extrusion_name, const Xtring &text, bool addnewline=true);
    int insert_extrusion_at(int begin, const Xtring &extrusion_name, const Xtring &text,
      const Xtring &default_following_text=Xtring(),
      const Xtring &default_preceding_text=Xtring(), bool addnewline=true);
    int insert_extrusion(const Xtring &extrusion_name, const Xtring &text,
      const Xtring &default_following_text=Xtring(),
      const Xtring &default_preceding_text=Xtring(), bool addnewline=true)
      { return insert_extrusion_at( -2, extrusion_name, text,
          default_following_text, default_preceding_text, addnewline); }
    int delete_extrusion(const Xtring &extrusion_name);
    int empty_extrusion(const Xtring &extrusion_name);
    int position_before_extrusion(const Xtring &extrusion_name);
    int position_after_extrusion(const Xtring &extrusion_name);
    int position_before_text(const Xtring &extrusion_name);
    int position_after_text(const Xtring &extrusion_name);
    int position_begin() const { return 0; }
    int new_extrusion(const Xtring &extrusion_name) { return empty_extrusion(extrusion_name); };
    Xtring &replace_global(const Xtring &search, const Xtring &repl);
    Xtring &replace_in_extrusion(const Xtring &extrusion_name, const Xtring &search, 
				  const Xtring &repl);
    Xtring::size_type find_global(const Xtring &search);
    Xtring get_current_extrusion() { return mCurrentExtrusion; };
    void set_current_extrusion(const Xtring &extrusion_name) { mCurrentExtrusion = extrusion_name; };
    CapelModule &operator<<(const Xtring &);
    CapelModule &operator<<(int);
	Xtring capitalize( const Xtring &str );
    Xtring mBuffer;

protected:
    Xtring mFilename;
    Xtring mBeginComment;
    Xtring mEndComment;
    Xtring mBeginKeyword;
    Xtring mEndKeyword;
    Xtring mCurrentExtrusion;

    Xtring fill_begin_extrusion(const Xtring &extrusion_name);
    Xtring fill_end_extrusion(const Xtring &extrusion_name);
    Xtring::size_type lookup_extrusion_begin(const Xtring &extrusion_name);
    Xtring::size_type lookup_extrusion_code(const Xtring &extrusion_name);
    Xtring::size_type lookup_extrusion_end(const Xtring &extrusion_name);
    Xtring::size_type insert_text(int begin, const Xtring &text, bool addnewline=true);    
};

}

#endif
